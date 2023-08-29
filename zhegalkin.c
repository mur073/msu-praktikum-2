#include <stdio.h>
#include <stdlib.h>

int fast_pow(int a, int n);

char *read_data(FILE *fin, int *n, int *k, int *size);
char *zhegalkin(int n, int size, char *value);
char xorc(char x, char y);

void print_polynomial(FILE *fout, int n, int size, char *coefs);

int main(int argc, char *argv[]) {
    if (argc == 3) {
        FILE *fin = fopen(argv[1], "r");
        FILE *fout = fopen(argv[2], "w");

        int n = -1, k = -1, size = -1;
        char *value = NULL, *coefs = NULL;

        if (fin) {
            value = read_data(fin, &n, &k, &size);
            fclose(fin);
        }

        if (value) {
            coefs = zhegalkin(n, size, value);
            free(value);
        } else {
            printf("smth went wrong while reading intput file...\n");
            return 0;
        }

        if (fout) {
            print_polynomial(fout, n, size, coefs);
            fclose(fout);
        }

        free(coefs);
    }

    return 0;
}

char xorc(char x, char y) {
    int X = x - 48;
    int Y = y - 48;

    return (X ^ Y) ? '1' : '0';
}

char *zhegalkin(int n, int size, char *value) {
    char **matrix = (char**)malloc(sizeof(char*) * size);
    char *arr = (char*)malloc(sizeof(char) * size);

    for (int i = 0; i < size; ++i) matrix[i] = (char*)malloc(sizeof(char) * size);

    for (int i = 0; i < size; ++i) matrix[i][0] = '0';

    for (int i = 0; i < size; ++i) matrix[0][i] = value[i];

    arr[0] = matrix[0][0];

    for (int i = 1; i < size; ++i) {
        for (int j = 1; j < size; ++j) {
            matrix[i][j] = xorc(matrix[i - 1][j - 1], matrix[i - 1][j]);

            if (i == j) arr[i] = matrix[i][j];
        }
    }

    for (int i = 0; i < size; ++i) free(matrix[i]);

    free(matrix);

    return arr;
}

void print_polynomial(FILE *fout, int n, int size, char *coefs) {
    int m = 0;
    for (int i = 0; i < size; ++i) if (coefs[i] == '1') ++m;

    fprintf(fout, "2 %d %d\n", n, m);

    for (int i = 0; i < size; ++i) {
        if (coefs[i] == '1') {
            int tmp = i, j = n - 1;
            while (j >= 0) {
                fprintf(fout, "%d", (tmp >> j) & 1);
                --j;
            }
            fprintf(fout, " 1\n");
        }
    }
}

char *read_data(FILE *fin, int *n, int *k, int *size) {
    if (fscanf(fin, "%d", k) > 0) {
        fscanf(fin, "%d", n);

        (*size) = fast_pow(*k, *n);

        char *value = (char*)malloc(sizeof(char) * (*size));
        
        char tmp;
        fscanf(fin, "%c", &tmp);

        for (int i = 0; i < *size; ++i) {
            fscanf(fin, "%c", (value+i));
        }

        return value;
    }

    return NULL;
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