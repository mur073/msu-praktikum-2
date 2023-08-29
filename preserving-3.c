#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int logic;

int fast_pow(int a, int n);
int equivalent_sets(int k, int n, short *q, short *v, short *partition);
int cmp(const void *p1, const void *p2);

short **find_const_partitions(int k, int n, int *partitions_cnt, short **table);
short **read(char *path, int *K, int *N);

void print_partitions(char *path, int k, int partitions_cnt, short **partitions);
void get_all_partitions(int k, int index, short *part, int partitions_cnt, short **partitions);


int main(int argc, char *argv[]) {
    if (argc == 3) {
        int k = 0, n = 0;

        short **table = read(argv[1], &k, &n);

        if (table) {
            int kn = fast_pow(k, n), partitions_cnt;

            short **partitions = find_const_partitions(k, n, &partitions_cnt, table);

            for (int i = 0; i < kn; ++i) free(table[i]); free(table);

            print_partitions(argv[2], k, partitions_cnt, partitions);

            for (int i = 0; i < partitions_cnt; ++i) free(partitions[i]); free(partitions);
        }
    }
    
    return 0;
}

short **find_const_partitions(int k, int n, int *partitions_cnt, short **table) {
    int bell[] = {1, 1, 2, 5, 15, 52, 203, 877, 4140, 21147, 115975};

    *partitions_cnt = bell[k];
    short **partitions = (short**)malloc(sizeof(short*) * (*partitions_cnt + 1));

    for (int i = 0; i < *partitions_cnt; ++i) {
        partitions[i] = (short*)malloc(sizeof(short) * (k + 2));
        partitions[i][k + 1] = 1;
    }

    partitions[*partitions_cnt] = (short*)malloc(sizeof(short));
    partitions[*partitions_cnt][0] = 0;

    short *part = (short*)malloc(sizeof(short) * (k + 1));

    for (int i = 0; i <= k; ++i) part[i] = 0;

    get_all_partitions(k, 0, part, *partitions_cnt, partitions);

    free(part);

    int kn = fast_pow(k, n);

    for (int i = 0; i < kn - 1; ++i) {
        for (int j = i + 1; j < kn; ++j) {
            for (int t = 1; t < *partitions_cnt - 1; ++t) {
                if (partitions[t][k + 1] == -1) continue;
                int condition1 = equivalent_sets(k, n, table[i], table[j], partitions[t]);
                int condition2 = equivalent_sets(k, 1, table[i]+n, table[j]+n, partitions[t]);

                if (condition1 && !condition2) partitions[t][k + 1] = -1;
            }
        }
    }

    return partitions;
}

int equivalent_sets(int k, int n, short *q, short *v, short *partition) {
    int flag, symbol = n - 1;

    while (symbol >= 0) {
        flag = 0;
        for (int i = 0; i < partition[k]; ++i) {
            int condition1 = (partition[i] >> (k - q[symbol] - 1)) & 1;
            int condition2 = (partition[i] >> (k - v[symbol] - 1)) & 1;

            if (condition1 && condition2) { flag = 1; break; }
        }

        if (flag) --symbol; else return 0;
    }

    return 1;
}

void get_all_partitions(int k, int index, short *part, int partitions_cnt, short **partitions) {
    if (index == k) {
        short *sz = partitions[partitions_cnt];
        for (int i = 0; i <= k; ++i) partitions[*sz][i] = part[i];

        ++(*sz);

        return;
    }

    for (int i = 0; i < part[k]; ++i) {
        part[i] |= (1 << (k - index - 1));

        get_all_partitions(k, index + 1, part, partitions_cnt, partitions);

        part[i] &= ~(1 << (k - index - 1));
    }

    part[part[k]] |= (1 << (k - index - 1));
    ++part[k];

    get_all_partitions(k, index + 1, part, partitions_cnt, partitions);

    --part[k];
    part[part[k]] &= ~(1 << (k - index - 1));
}


void print_partitions(char *path, int k, int partitions_cnt, short **partitions) {
    FILE *fout = fopen(path, "w");

    if (!fout) {
        printf("Couldn't open output file...\n");
        return;
    }

    qsort(partitions, partitions_cnt, sizeof(*partitions), cmp);

    for (int i = partitions_cnt - 1; i >= 0; --i) {
        if (partitions[i][k + 1] == -1) continue;

        for (int j = 0; j < partitions[i][k]; ++j) {
            if (j) fprintf(fout, " ");
            int idx = k-1;
            while (idx >= 0) {
                if ((partitions[i][j] >> idx) & 1) {
                    fprintf(fout, "%hi", k-idx-1);
                }
                --idx;

            }
        }
        fprintf(fout, "\n");
    }

    fclose(fout);   
}

short **read(char *path, int *K, int *N) {
    FILE *fin = fopen(path, "r");

    if (!fin) {
        printf("Couldn't open input file...\n");
        return NULL;
    }

    fscanf(fin, "%d %d ", K, N); int n = *N, k = *K;

    if (k > 10) {
        printf("Maximum k value is 10!\n");
        return NULL;
    }

    logic = k;

    int kn = fast_pow(k, n);
    short **table = (short**)malloc(sizeof(short*) * kn);
 
    for (int i = 0; i < kn; ++i) table[i] = (short*)malloc(sizeof(short) * (n + 1));

    for (int i = 0; i < n; ++i) table[0][i] = 0;

    for (int i = 1; i < kn; ++i) {
        int j = n - 1;

        while (j >= 0 && table[i - 1][j] == (k - 1)) { table[i][j] = 0; --j; }

        table[i][j] = table[i - 1][j] + 1;
        --j;
        while (j >= 0) { table[i][j] = table[i - 1][j]; --j; }
    }

    for (int i = 0; i < kn; ++i) {
        fscanf(fin, "%c", table[i]+n);
        table[i][n] -= '0';
    }

    fclose(fin);

    return table;
}

int cmp(const void *p1, const void *p2) {
    short *a = *((short**)p1);
    short *b = *((short**)p2);

    short *x = (short*)malloc(sizeof(short) * logic);
    short *y = (short*)malloc(sizeof(short) * logic);

    int i = 0;
    while (a[i] && b[i]) {
        int j = 0;
        int tmp = 0;
        while (tmp < logic) {
            if ((a[i] >> (logic - tmp - 1)) & 1) {
                x[j] = logic - tmp - 1;
                ++j;
            }
            ++tmp;
        }

        int k = 0;
        tmp = 0;
        while (tmp < logic) {
            if ((b[i] >> (logic - tmp - 1)) & 1) {
                y[k] = logic - tmp - 1;
                ++k;
            }
            ++tmp;
        }

        int t = 0;
        while (t < j && t < k) {
            if (x[t] > y[t]) { free(x); free(y); return 1; }
            if (x[t] < y[t]) { free(x); free(y); return -1; }
            ++t;
        }

        if (j > k) { free(x); free(y); return -1; }
        if (j < k) { free(x); free(y); return 1; }

        ++i;
    }

    return 0;
}

int fast_pow(int a, int n) {
    int res = 1;
    while (n) {
        if (n & 1) res *= a;

        a *= a;

        n >>= 1;
    }
    return res;
}