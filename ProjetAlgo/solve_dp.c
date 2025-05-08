/********************************************************************
 * solve_dp.c
 *
 * Usage:
 *   ./solve_dp <inputFile> <outputFile>
 *
 * Description:
 *   Lit les instances de chaînes de matrices depuis <inputFile>,
 *   calcule la parenthésation optimale via la programmation dynamique,
 *   et écrit les résultats dans <outputFile>.
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAXN 100
#define MAXREPR 500

long long dp[MAXN][MAXN];
int split[MAXN][MAXN];
int d[MAXN + 1];

/* Reconstruit la parenthésation optimale */ 
void build_repr(int i, int j, char *repr) {
    static char buffer[MAXREPR];
    if (i == j) {
        sprintf(buffer, "M%d", i);
        strcat(repr, buffer);
        return;
    }
    strcat(repr, "(");
    build_repr(i, split[i][j], repr);
    build_repr(split[i][j] + 1, j, repr);
    strcat(repr, ")");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <inputFile> <outputFile>\n", argv[0]);
        return 1;
    }

    FILE *fin = fopen(argv[1], "r");
    FILE *fout = fopen(argv[2], "w");
    int T, n;

    fscanf(fin, "%d", &T);
    for (int inst = 0; inst < T; inst++) {
        fscanf(fin, "%d", &n);
        for (int i = 0; i <= n; i++) fscanf(fin, "%d", &d[i]);

        clock_t start = clock();
        memset(dp, 0, sizeof(dp));
        memset(split, 0, sizeof(split));

        for (int len = 2; len <= n; len++) {
            for (int i = 1; i <= n - len + 1; i++) {
                int j = i + len - 1;
                dp[i][j] = LLONG_MAX;
                for (int k = i; k < j; k++) {
                    long long cost = dp[i][k] + dp[k+1][j] + (long long)d[i-1] * d[k] * d[j];
                    if (cost < dp[i][j]) {
                        dp[i][j] = cost;
                        split[i][j] = k;
                    }
                }
            }
        }

        char bestTree[MAXREPR] = "";
        build_repr(1, n, bestTree);
        clock_t end = clock();
        double timeSec = (double)(end - start) / CLOCKS_PER_SEC;
        fprintf(fout, "%d %.6f %lld %s\n", n, (double)(end - start)/CLOCKS_PER_SEC, dp[1][n], bestTree);
        printf("[Progress] Instance %d: n=%d, time=%.6f, cost=%lld\n", inst+1, n, timeSec, dp[1][n]);
    }

    fclose(fin); fclose(fout);
    return 0;
}