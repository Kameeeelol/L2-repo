#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX 1024

// Structure contenant le coût et la représentation de la parenthésation
typedef struct {
    long long cost;
    char rep[MAX];
} Result;

// Fonction pour calculer le coût d’une multiplication A_i..A_k..A_j
long long mul_cost(int p[], int i, int k, int j) {
    return (long long)p[i] * p[k] * p[j];
}

// Fonction récursive gloutonne qui remplit une structure Result
Result hu_shing(int p[], int i, int j) {
    Result res;
    if (j - i <= 1) {
        res.cost = 0;
        snprintf(res.rep, MAX, "A%d", i + 1);
        return res;
    }

    long long min_cost = LLONG_MAX;
    int best_k = -1;

    for (int k = i + 1; k < j; ++k) {
        long long c = mul_cost(p, i, k, j);
        if (c < min_cost) {
            min_cost = c;
            best_k = k;
        }
    }

    Result left = hu_shing(p, i, best_k);
    Result right = hu_shing(p, best_k, j);

    res.cost = min_cost + left.cost + right.cost;
    snprintf(res.rep, MAX, "(%s x %s)", left.rep, right.rep);

    return res;
}

int main() {
    // Dimensions : A1: 10x30, A2: 30x5, A3: 5x60, A4: 60x10
    int p[] = {5, 10, 20, 40, 80};
    int n = sizeof(p) / sizeof(p[0]) - 1;

    Result result = hu_shing(p, 0, n);

    printf("Parenthésation optimale approximée : %s\n", result.rep);
    printf("Coût total approximatif : %lld\n", result.cost);

    return 0;
}
