#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAXN 100
#define MAXREPR 500

int d[MAXN + 1];
int n;

//dans ce cas là avec cette heuristique on essaie de diminuer la dimension de la matrice intermediaire 

long long greedy_ratio(int d[], int n, char *repr_final) {
    int dims[n + 1];
    char repr[n][MAXREPR];

    // Initialisation correcte des dimensions et des noms des matrices
    for (int i = 0; i <= n; i++) dims[i] = d[i];
    for (int i = 0; i < n; i++) sprintf(repr[i], "M%d", i + 1);

    long long total_cost = 0;
    int remaining = n;

    while (remaining > 1) {
        int best_pos = 0;
        double min_ratio = (double)dims[1] / (dims[0] < dims[2] ? dims[0] : dims[2]);

        // Trouver la paire adjacente avec le meilleur ratio
        for (int i = 0; i < remaining - 1; i++) {
            int left = dims[i];
            int middle = dims[i + 1];
            int right = (i + 2 < remaining) ? dims[i + 2] : INT_MAX; // Gestion des bords
            double ratio = (double)middle / (left < right ? left : right);

            if (ratio < min_ratio) {
                min_ratio = ratio;
                best_pos = i;
            }
        }

        // Calcul du coût
        long long cost = (long long)dims[best_pos] * dims[best_pos + 1] * dims[best_pos + 2];
        total_cost += cost;

        // Fusion des représentations
        char temp[MAXREPR];
        snprintf(temp, MAXREPR, "(%s*%s)", repr[best_pos], repr[best_pos + 1]);
        strcpy(repr[best_pos], temp);

        // Mise à jour des dimensions et des représentations
        for (int i = best_pos + 1; i < remaining - 1; i++) {
            dims[i] = dims[i + 1];
            strcpy(repr[i], repr[i + 1]);
        }
        remaining--;
    }

    if (repr_final) strcpy(repr_final, repr[0]);
    return total_cost;
}



int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <inputFile> <outputFile>\n", argv[0]);
        return 1;
    }

    const char *inFile  = argv[1];
    const char *outFile = argv[2];

    FILE *fin = fopen(inFile, "r");
    if (!fin) {
        fprintf(stderr, "Cannot open input file: %s\n", inFile);
        return 1;
    }
    FILE *fout = fopen(outFile, "w");
    if (!fout) {
        fprintf(stderr, "Cannot open output file: %s\n", outFile);
        fclose(fin);
        return 1;
    }

    int T;
    fscanf(fin, "%d", &T);

    for (int inst = 0; inst < T; inst++) {
        fscanf(fin, "%d", &n);
        for (int i = 0; i <= n; i++) fscanf(fin, "%d", &d[i]);

        char repr_final[MAXREPR] = {0};

        clock_t start = clock();
        
        long long best_cost = greedy_ratio(d, n, repr_final);

        clock_t end = clock();
        double timeSec = (double)(end - start) / CLOCKS_PER_SEC;

        fprintf(fout, "%d %.6f %lld %s\n", n, timeSec, best_cost, repr_final);  // Écrire aussi repr_final
        fflush(fout);

        printf("[Progress] Solved instance by greedy_aris %d / %d : n=%d, time=%.6f, bestCost=%lld\n",
               inst + 1, T, n, timeSec, best_cost);
    }

    fclose(fin);
    fclose(fout);
    printf("Processed %d instances. Results saved to %s\n", T, outFile);
    return 0;
}






