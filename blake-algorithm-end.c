#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int can_absorb(int n, char *k, char *l);
int fast_pow(int a, int n);

void write_data(FILE *fout, int n, int m, char **dnf);

void min_dnf(int n, int m, char **dnf);
char **read_data(FILE *fin, int *n, int *m);


int main(int argc, char *argv[]) {
    if (argc == 3) {
        FILE *fin = fopen(argv[1], "r");
        FILE *fout = fopen(argv[2], "w");

        int n = 0, m = 0;
        char **dnf = NULL;
        
        if (fin) {
            dnf = read_data(fin, &n, &m);
            fclose(fin);
        }
        
        if (dnf) {
            min_dnf(n, m, dnf);
            
            if (fout) {
                write_data(fout, n, m, dnf);
                fclose(fout);
            }
            
            for (int i = 0; i < m; ++i) free(dnf[i]);
            free(dnf);
        }
    }

    return 0;
}

void min_dnf(int n, int m, char **dnf) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            if (i == j) continue;
            if (can_absorb(n, dnf[i], dnf[j])) {
                dnf[j][n] = '-';
            }
        }
    }
}

int can_absorb(int n, char *k, char *l) {
    for (int i = 0; i < n; ++i) {
        if (k[i] == '*') continue;
        
        if (k[i] != l[i]) return 0;
    }
    
    return 1;
}

char **read_data(FILE *fin, int *n, int *m) {
    if (fscanf(fin, "%d %d", n, m) == 2) {
        char **dnf = (char**)malloc(sizeof(char*) * (*m));

        char tmp;

        while(fscanf(fin, "%c", &tmp) > 0 && tmp != '\n');

        for (int i = 0; i < *m; ++i) {
            dnf[i] = (char*)malloc(sizeof(char) * (*n + 1));
            dnf[i][*n] = '+';

            for (int j = 0; j < *n; ++j) {
                fscanf(fin, "%c", &tmp);
                dnf[i][j] = tmp;
            }

            while(fscanf(fin, "%c", &tmp) > 0 && tmp != '\n');
        }

        return dnf;
    }
    return NULL;
}

void write_data(FILE *fout, int n, int m, char **dnf) {
    int conj_cnt = 0;
    
    for (int i = 0; i < m; ++i) conj_cnt += (dnf[i][n] == '+') ? 1 : 0;
    
    fprintf(fout, "%d %d\n", n, conj_cnt);
    
    for (int i = 0; i < m; ++i) {
        if (dnf[i][n] == '+') {
            for (int j = 0; j < n; ++j) fprintf(fout, "%c", dnf[i][j]);
            fprintf(fout, "\n");
        }
    }
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