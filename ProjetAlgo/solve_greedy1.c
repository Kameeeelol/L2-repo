/* ******************************************************************
 * solve_greedy1.c
 *
 * Usage:
 *   ./solve_greedy1 <inputFile> <outputFile>
 *
 * Description:
 *   Lit les instances de chaînes de matrices depuis <inputFile>,
 *   calcule une parenthésation approximative via l'heuristique gloutonne 1 :
 *   fusionner les paires adjacentes les moins coûteuses,
 *   et écrit les résultats (taille, temps, coût, parenthésation) dans <outputFile>.
 ********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAXN    100
#define MAXREPR 500

// Structure pour renvoyer le résultat
typedef struct {
    long long cost;
    char repr[MAXREPR];
} Result;

// Heuristique Glouton 1 : fusionner la paire adjacente de coût minimal
Result greedy1(int d[], int n) {
    // Copie des dimensions
    int dims[MAXN+1];
    memcpy(dims, d, (n+1)*sizeof(int));

    // Labels pour chaque matrice
    char *labels[MAXN];
    for (int i = 0; i < n; i++) {
        labels[i] = malloc(MAXREPR);
        snprintf(labels[i], MAXREPR, "M%d", i+1);
    }

    int mats = n;            // nombre de matrices restantes
    int len = n + 1;         // longueur de dims
    long long total_cost = 0;

    // Tant qu'il reste plus d'une matrice, on fusionne
    while (mats > 1) {
        // Chercher la paire adjacente de coût minimal
        long long min_cost = LLONG_MAX;
        int best_i = 0;
        for (int i = 1; i < len - 1; i++) {
            long long c = (long long)dims[i-1] * dims[i] * dims[i+1];
            if (c < min_cost) {
                min_cost = c;
                best_i = i;
            }
        }
        // fusionner la paire best_i-1 et best_i
        total_cost += min_cost;

        // construire nouveau label
        char newlab[MAXREPR];
        snprintf(newlab, MAXREPR, "(%s%s)", labels[best_i-1], labels[best_i]);
        // mettre à jour labels
        free(labels[best_i-1]);
        labels[best_i-1] = malloc(MAXREPR);
        strncpy(labels[best_i-1], newlab, MAXREPR);
        free(labels[best_i]);
        // décaler labels
        for (int j = best_i; j < mats-1; j++) {
            labels[j] = labels[j+1];
        }

        // mettre à jour dims
        // dims[best_i] devient dims[best_i+1]
        dims[best_i] = dims[best_i+1];
        // décaler
        for (int j = best_i+1; j < len-1; j++) {
            dims[j] = dims[j+1];
        }
        mats--;
        len--;
    }

    // Préparer le résultat
    Result res;
    res.cost = total_cost;
    strncpy(res.repr, labels[0], MAXREPR);
    // libérer dernier label
    free(labels[0]);
    return res;
}


/*int main() {
    // Exemple : d = [10, 100, 5, 50, 20] => M1(10x100), M2(100x5), etc.
    int d[] = {5, 100, 2, 50, 10};
    int n = sizeof(d) / sizeof(d[0]) -1;

    Result *test = malloc(sizeof(Result));
    *test = greedy1(d, n);

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
    if (!fin || !fout) { perror("fopen"); return 1; }

    int T, n;
    int d[MAXN+1];
    fscanf(fin, "%d", &T);

    for (int inst = 0; inst < T; inst++) {
        fscanf(fin, "%d", &n);
        for (int i = 0; i <= n; i++) fscanf(fin, "%d", &d[i]);

        clock_t start = clock();
        Result r = greedy1(d, n);
        clock_t end = clock();
        double timeSec = (double)(end - start) / CLOCKS_PER_SEC;

        fprintf(fout, "%d %.6f %lld %s\n", n, timeSec, r.cost, r.repr);
        printf("[Progress] Instance %d: n=%d, time=%.6f, cost=%lld\n",
               inst+1, n, timeSec, r.cost);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
