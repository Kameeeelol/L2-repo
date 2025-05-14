/********************************************************************
 * solve_maxdim.c
 *
 * Usage:
 *   ./solve_maxdim <inputFile> <outputFile>
 *
 * Description:
 *   Lit les instances depuis <inputFile>,
 *   applique l'heuristique “élimination de la plus grande dimension”,
 *   et écrit n, temps, coût et parenthésation dans <outputFile>.
 ********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAXN    100
#define MAXREP  500

typedef struct {
    long long cost;
    char repr[MAXREP];
} Result;

// Cherche la position k de la plus grande dimension d[1..n-1]
static int find_max_dim(int d[], int n) {
    int k = 1;
    for (int i = 2; i < n; ++i)
        if (d[i] > d[k]) k = i;
    return k;
}

// Heuristique “max-dimension”
Result greedy_maxdim(int d_orig[], int n) {
    int d[MAXN+1];
    memcpy(d, d_orig, sizeof(int)*(n+1));

    // Labels initiaux M1..Mn
    char *labels[MAXN];
    for (int i = 0; i < n; ++i) {
        labels[i] = malloc(MAXREP);
        snprintf(labels[i], MAXREP, "M%d", i+1);
    }

    long long total_cost = 0;
    int len = n+1, mats = n;

    while (mats > 1) {
        int k = find_max_dim(d, len);
        // déterminer fusion i=k-1 ou i=k selon extrémités
        int i = (k==len-1 ? k-1 : k);
        // coût et fusion
        total_cost += (long long)d[i-1]*d[i]*d[i+1];
        // mise à jour repr.
        char buf[MAXREP];
        snprintf(buf, MAXREP, "(%s%s)", labels[i-1], labels[i]);
        free(labels[i-1]);
        labels[i-1] = strdup(buf);
        free(labels[i]);
        // décalage des labels et des dimensions
        for (int j = i; j < mats-1; ++j) labels[j] = labels[j+1];
        for (int j = i; j < len-1; ++j)   d[j] = d[j+1];
        mats--; len--;
    }

    Result res = { total_cost, "" };
    strncpy(res.repr, labels[0], MAXREP);
    free(labels[0]);
    return res;
}

/*int main() {
    // Exemple : d = [10, 100, 5, 50, 20] => M1(10x100), M2(100x5), etc.
    int d[] = {3, 8, 2, 7, 4, 6};
    int n = sizeof(d) / sizeof(d[0]) -1;

    Result *result=malloc(sizeof(Result));
    *result = greedy_maxdim(d, n);

    printf("Parenthésation gloutonne 1 : %s\n", result->repr);
    printf("Coût total estimé : %lld\n", result->cost);

    return 0;
}*/


int main(int argc, char *argv[]) {
    if (argc<3) { fprintf(stderr,"Usage: %s <in> <out>\n",argv[0]); return 1; }
    FILE *fin=fopen(argv[1],"r"),*fout=fopen(argv[2],"w");
    int T,n; int d[MAXN+1];
    fscanf(fin,"%d",&T);
    for(int t=0;t<T;++t){
        fscanf(fin,"%d",&n);
        for(int i=0;i<=n;++i) fscanf(fin,"%d",&d[i]);
        clock_t s=clock();
        Result r=greedy_maxdim(d,n);
        double tm=(double)(clock()-s)/CLOCKS_PER_SEC;
        fprintf(fout,"%d %.6f %lld %s\n",n,tm,r.cost,r.repr);
        printf("[Inst %d] n=%d time=%.6f cost=%lld\n",t+1,n,tm,r.cost);
    }
    fclose(fin); fclose(fout);
    return 0;
}
