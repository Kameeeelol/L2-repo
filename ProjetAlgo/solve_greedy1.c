/********************************************************************
 * solve_greedy1.c
 *
 * Usage:
 *   ./solve_greedy1 <inputFile> <outputFile>
 *
 * Description:
 *   Calcule le coût via l'heuristique gloutonne 1 : fusionner les paires adjacentes les moins coûteuses.
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAXN 100
#define MAXREPR 500

typedef struct {
    int start, end;
} Pair;

long long compute_cost(int d[], int n, char *repr) {
    int *dims = malloc((n + 1) * sizeof(int));
    Pair *history = malloc((n - 1) * sizeof(Pair));
    for (int i = 0; i <= n; i++) dims[i] = d[i];
    int current_n = n;
    long long total_cost = 0;

    // Fusionner les paires
    for (int step = 0; step < n - 1; step++) {
        int min_cost = INT_MAX, best_idx = 0;

        // Trouver la meilleure paire
        for (int i = 0; i < current_n - 1; i++) {
            int cost = dims[i] * dims[i + 1] * dims[i + 2];
            if (cost < min_cost) {
                min_cost = cost;
                best_idx = i;
            }
        }

        // Enregistrer l'historique
        history[step] = (Pair){best_idx + 1, best_idx + 2}; // 1-based

        // Mettre à jour les dimensions
        dims[best_idx + 1] = dims[best_idx + 2];
        for (int i = best_idx + 2; i < current_n; i++) dims[i] = dims[i + 1];
        current_n--;
        total_cost += min_cost;
    }

    // Reconstruire la parenthesation
    char buffer[MAXREPR] = "";
    for (int i = n - 2; i >= 0; i--) {
        char temp[MAXREPR];
        sprintf(temp, "(%sM%d:M%d)", buffer, history[i].start, history[i].end);
        strcpy(buffer, temp);
    }
    strcpy(repr, buffer);

    free(dims); free(history);
    return total_cost;
}

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
        char bestTree[MAXREPR] = "";
        long long cost = compute_cost(d, n, bestTree);
        clock_t end = clock();
        double timeSec = (double)(end - start) / CLOCKS_PER_SEC;

        fprintf(fout, "%d %.6f %lld %s\n", n, timeSec, cost, bestTree);
        printf("[Progress] Instance %d: n=%d, time=%.6f, cost=%lld\n", inst+1, n, timeSec, cost);
    }

    fclose(fin); fclose(fout);
    return 0;
}