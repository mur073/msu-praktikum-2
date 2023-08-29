#include <stdio.h>
#include <stdlib.h>

#define eint int

eint **read_data(FILE *fin, int *n);
eint fast_pow(int a, int n);

char foo(int u, int v, eint **graph, char **matrix);
void find_equal_states(FILE *fout, int n, eint **graph);
void printb(FILE *fout, int n, int cnt);

int main(int argc, char *argv[]) {
    if (argc == 3) {
        FILE *fin = fopen(argv[1], "r");
        FILE *fout = fopen(argv[2], "w");

        int n, kn;
        eint **graph = NULL;
        if (fin) {
            graph = read_data(fin, &n);
            kn = fast_pow(2, n + 1);
            fclose(fin);
        }

        if (fout) {
            if (graph) {
                find_equal_states(fout, n, graph);

                for (int i = 0; i < kn; ++i) free(graph[i]);

                free(graph);
            }

            fclose(fout);
        }
    }
    
    return 0;
}

void find_equal_states(FILE *fout, int n, eint **graph) {
    int states_cnt = fast_pow(2, n);
    char **matrix = (char**)malloc(sizeof(char*) * states_cnt);

    for (int i = 0; i < states_cnt; ++i) {
        matrix[i] = (char*)malloc(sizeof(char) * states_cnt);
        for (int j =  0; j < states_cnt; ++j) {
            matrix[i][j] = '-';
        }
    }

    int changing = 1;

    while (changing) {
        changing = 0;
        for (int i = 1; i < states_cnt; ++i) {
            for (int j = 0; j < i; ++j) {
                if (matrix[i][j] == '-') {
                    matrix[i][j] = foo(i, j, graph, matrix);

                    if (matrix[i][j] == '+') changing = 1;
                }
            }
        }
    }

    int found = 0;
    for (int i = 0; i < states_cnt - 1; ++i) {
        for (int j = i + 1; j < states_cnt; ++j) {
            if (matrix[j][i] == '-') {
                found = 1;
                printb(fout, i, n);
                printb(fout, j, n);
            }
        }
    }

    if (!found) fprintf(fout, "-1\n");

    for (int i = 0; i < states_cnt; ++i) free(matrix[i]);

    free(matrix);
}

void printb(FILE *fout, int n, int cnt) {
    for (int i = 0; i < cnt; ++i) fprintf(fout, "%d", (n >> (cnt - i - 1)) & 1);
    fprintf(fout, " ");
}

char foo(int u, int v, eint **graph, char **matrix) {
    for (int i = 0; i < 2; ++i) {
        int q_u = graph[u][i];
        int q_v = graph[v][i];
        
        if (matrix[q_u >> 1][q_v >> 1] == '+' || matrix[q_v >> 1][q_u >> 1] == '+') return '+';

        if ((q_u + q_v) & 1) return '+';
    }

    return '-';
}

eint **read_data(FILE *fin, int *n) {
    if (fscanf(fin, "%d", n) > 0) {
        int kn = fast_pow(2, *n + 1), k, cnt;
        char tmp;

        eint **graph = (eint**)malloc(sizeof(eint*) * kn);

        if (!graph) return NULL;

        for (int i = 0; i < kn; ++i) {
            graph[i] = (eint*)malloc(sizeof(eint) * 2);
            graph[i][0] = 0; graph[i][1] = 0;
        }

        // scanf for init state
        fscanf(fin, "%c", &tmp);
        for (int i = 0; i < *n; ++i) fscanf(fin, "%c", &tmp);

        // scanf for tansition functions
        for (int i = 0; i < *n; ++i) {
            fscanf(fin, "%d %d%c", &k, &cnt, &tmp);

            cnt = 0;
            for (int j = 0; j < kn; ++j) {
                fscanf(fin, "%c", &tmp);
                if (tmp == '1') graph[j >> 1][j & 1] |= (1 << (*n - i));
            }
        }

        fscanf(fin, "%d %d%c", &k, &cnt, &tmp);

        // scanf for f(x1, ..., xn) value
        for (int i = 0; i < kn; ++i) {
            fscanf(fin, "%c", &tmp);
            if (tmp == '1') graph[i >> 1][i & 1] |= 1;
        }

        return graph;
    }
    
    return NULL;
}

eint fast_pow(int a, int n) {
    eint res = 1;
    while (n) {
        if (n & 1) res *= 1LL * a;

        a *= a;

        n >>= 1;
    }
    return res;
}