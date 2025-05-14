/********************************************************************
 * solve_greedy2.c
 *
 * Usage:
 *   ./solve_greedy2 <inputFile> <outputFile>
 *
 * Description:
 *   Calcule le coût via l'heuristique gloutonne 2 : diviser au coût frontal minimal.
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAXN 100
#define MAXREPR 500

typedef struct {
    long long cost;
    char repr[MAXREPR];
} Result;

Result greedy2(int d[], int i, int j) {
    Result res;
    if (i == j) {
        res.cost = 0;
        sprintf(res.repr, "M%d", i);
        return res;
    }

    int best_k = i;
    int min_front = INT_MAX;
    for (int k = i; k < j; k++) {
        int front_cost = d[i-1] * d[k] * d[j];
        if (front_cost < min_front) {
            min_front = front_cost;
            best_k = k;
        }
    }

    Result left = greedy2(d, i, best_k);
    Result right = greedy2(d, best_k + 1, j);
    res.cost = left.cost + right.cost + (long long)d[i-1] * d[best_k] * d[j];
    sprintf(res.repr, "(%s%s)", left.repr, right.repr);
    return res;
}

/* int main() {
    // Exemple : d = [10, 100, 5, 50, 20] => M1(10x100), M2(100x5), etc.
    int d[] = {5, 100, 2, 50, 10};
    int n = sizeof(d) / sizeof(d[0]) -1;

    Result *test = malloc(sizeof(Result));
    *test = greedy2(d, 1, n);

    printf("Parenthésation gloutonne 1 : %s\n", test->repr);
    printf("Coût total estimé : %lld\n", test->cost);

    return 0;
}*/


int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <inputFile> <outputFile>\n", argv[0]);
        return 1;
    }

    FILE *fin = fopen(argv[1], "r");
    FILE *fout = fopen(argv[2], "w");
    int T, n, d[MAXN + 1];

    fscanf(fin, "%d", &T);
    for (int inst = 0; inst < T; inst++) {
        fscanf(fin, "%d", &n);
        if (n > MAXN) {
            fprintf(stderr, "n=%d dépasse MAXN=%d\n", n, MAXN);
            exit(1);
        }
        for (int i = 0; i <= n; i++) fscanf(fin, "%d", &d[i]);

        clock_t start = clock();
        Result res = greedy2(d, 1, n); // Indices 1-based
        clock_t end = clock();
        double timeSec = (double)(end - start) / CLOCKS_PER_SEC;

        fprintf(fout, "%d %.6f %lld %s\n", n, timeSec, res.cost, res.repr);
        printf("[Progress] Instance %d: n=%d, time=%.6f, cost=%lld\n", inst+1, n, timeSec, res.cost);
    }

    fclose(fin); fclose(fout);
    return 0;
}