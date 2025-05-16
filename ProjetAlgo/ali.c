#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAXN 100
#define MAXchaine 500

// Fonction qui trouve la position i du coût de multiplication le plus bas
int find_best_k(int d[], int n) {
    long long bestCost = LLONG_MAX;
    int bestK = -1;

    for (int i = 1; i < n; i++) {
        long long cost = (long long)d[i - 1] * d[i] * d[i + 1];
        if (cost < bestCost) {
            bestCost = cost;
            bestK = i;
        }
    }
    return bestK;
}

// Supprime d[k] du tableau de dimensions (fusion entre M_k et M_{k+1})
void delete_at(int d[], int *n, int k) {
    for (int i = k; i < *n; i++) {
        d[i] = d[i + 1];
    }
    (*n)--;
}

// Supprime la k-ième chaîne (fusion) dans le tableau de chaînes
void merge_chaine(char chaine[][MAXchaine], int *n, int k) {
    char merged[MAXchaine];
    snprintf(merged, MAXchaine, "(%s %s)", chaine[k - 1], chaine[k]);
    
    // Replace chaine[k-1] with merged
    strcpy(chaine[k - 1], merged);

    // Shift left chaine[k+1...]
    for (int i = k; i < *n - 1; i++) {
        strcpy(chaine[i], chaine[i + 1]);
    }
    (*n)--;
}

long long greedy_with_trace(int d_init[], int n, char *finalchaine) {
    int d[MAXN + 1];
    int m = n;  // number of matrices
    int t[MAXN]; // table pour stocker les fusions
    int tlen = 0;

    for (int i = 0; i <= n; i++) d[i] = d_init[i];

    long long totalCost = 0;

    while (m > 1) {
        int k = find_best_k(d, m);
        long long cost = (long long)d[k - 1] * d[k] * d[k + 1];
        totalCost += cost;

        t[tlen++] = k;
        delete_at(d, &m, k);
    }

    // reconstruction du parenthésage à partir de t[]
    // initialise les chaineésentations
    char chaine[MAXN][MAXchaine];
    for (int i = 0; i < n; i++) {
        snprintf(chaine[i], MAXchaine, "M%d", i + 1);
    }

    int len = n;
    for (int i = 0; i < tlen; i++) {
        int k = t[i];
        merge_chaine(chaine, &len, k);
    }

    strcpy(finalchaine, chaine[0]);
    return totalCost;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <inputFile> <outputFile>\n", argv[0]);
        return 1;
    }

    const char *inFile = argv[1];
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
        int n;
        int d[MAXN + 1];
        fscanf(fin, "%d", &n);
        for (int i = 0; i <= n; i++) {
            fscanf(fin, "%d", &d[i]);
        }

        char chaine[MAXchaine] = "";
        clock_t start = clock();
        long long cost = greedy_with_trace(d, n, chaine);
        clock_t end = clock();

        double timeSec = (double)(end - start) / CLOCKS_PER_SEC;

        fprintf(fout, "%d %.6f %lld %s\n", n, timeSec, cost, chaine);
        fflush(fout);

        printf("[Progress] Instance %d/%d: n=%d, time=%.6f, cost=%lld\n", inst + 1, T, n, timeSec, cost);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}


