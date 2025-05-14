/********************************************************************
 * solve_backtracking.c
 *
 * Usage:
 *   ./solve_backtracking <inputFile> <outputFile>
 *
 * Description:
 *   Reads a set of matrix-chain instances from <inputFile>,
 *   enumerates all parenthesizations using a stack-based backtracking
 *   approach, finds the optimal cost and parenthesization, and
 *   writes results to <outputFile> with lines of the form:
 *
 *       n timeSeconds bestCost bestParenthesization
 *
 *   Prints progress on the number of instances solved.
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAXN 100
#define MAXSTACK 200
#define MAXREPR 500
#define MAXNBSOL 100000

typedef struct {
    int rows, cols;
    long long cost;    // 64-bit for safety
    char repr[MAXREPR];
} Node;


/* Global stack */
static Node stack[MAXSTACK];
static int top = 0; // stack pointer

/* We'll store the current dimensions in d[] for the chain */
static int d[MAXN+1];
static int n; // number of matrices


typedef struct {
	int n;
	double timeSec;
	long long bestCost;
	char res[MAXREPR];

} Results;

typedef struct {

	int nb_lecture;
	int accu;
	
} res_final;


static int nb_sol = 0;
static long long Costs[MAXNBSOL];


/* -------------- Stack operations -------------- */
static void push(Node nd)
{
    if (top >= MAXSTACK) {
        fprintf(stderr, "Stack overflow.\n");
        exit(1);
    }
    stack[top++] = nd;
}

static Node pop(void)
{
    if (top <= 0) {
        fprintf(stderr, "Stack underflow.\n");
        exit(1);
    }
    return stack[--top];
}

/* -------------- Leaf creation -------------- */
static Node makeLeaf(int i)
{
    Node leaf;
    leaf.rows = d[i-1];
    leaf.cols = d[i];
    leaf.cost = 0LL;
    snprintf(leaf.repr, sizeof(leaf.repr), "M%d", i);
    return leaf;
}

/* -------------- Combine two sub-products -------------- */
static Node combineNodes(const Node *A, const Node *B)
{
    Node C;
    long long combineCost = (long long)A->rows * A->cols * B->cols;
    C.rows = A->rows;
    C.cols = B->cols;
    C.cost = A->cost + B->cost + combineCost;
    snprintf(C.repr, sizeof(C.repr), "(%s %s)", A->repr, B->repr);
    return C;
}

/* -------------- Backtracking -------------- */
static void backtrack(int i)
{
    /* Case 1: SHIFT if i <= n */
    if (i <= n) {
        Node leaf = makeLeaf(i);
        push(leaf);
        backtrack(i + 1);
        pop(); // undo SHIFT
    }

    /* Case 2: REDUCE if top >= 2 */
    if (top >= 2) {
        Node B = pop();
        Node A = pop();
        Node C = combineNodes(&A, &B);

        push(C);
        backtrack(i);
        pop(); // undo REDUCE

        // restore
        push(A);
        push(B);
    }

    /* Case 3: check if complete parse => i>n & top==1 */
    if (i > n && top == 1) {
    
        Node candidate = stack[top-1];
        
	Costs[nb_sol++] = candidate.cost;
		    
    }
    
}

static int getEcart(long long cost) {

	for (int i=0; i<nb_sol; i++) {
		
		if (Costs[i] == cost)
			return i;
	
	}
	
	return nb_sol;  // solution non trouvée (ERREUR)	
}



static int compare_sols(const void * a, const void * b) {
	
	
	long long va =  * ((long long *) a);
	long long vb = * ((long long *) b);
	
	if (va < vb) return -1;
	
	if (va > vb) return +1; 
	
	return 0;
}



int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <inputFile>\n", argv[0]);
        return 1;
    }
    
    const char *inFile  = argv[1];
    const char *outFile  = argv[2];

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
    
    
    char * files[] = { "results_greedy1.txt", "results_greedy2.txt", "results_greedy3.txt" };
    char * names[] = { "greedy1",    "greedy2",    "greedy3" };
    
    FILE * algo[3];
    
    for (int i=0; i<3; i++) {
		algo[i] = fopen(files[i], "r");	
    }


    Results resultat[3] = {0};
    
    res_final res[3] = {0};
    

    // Read total number of instances T
    int T;
    fscanf(fin, "%d", &T);

    // We'll solve each instance, measure time, and record results.
    for (int inst = 0; inst < T; inst++) {
    
        // read n
        fscanf(fin, "%d", &n);
        // read n+1 dimensions
        for (int i = 0; i < n+1; i++) {
            fscanf(fin, "%d", &d[i]);
        }

        // measure time
        clock_t start = clock();

        // Reset stack
        top = 0;
        
        // run backtracking
      	nb_sol = 0;
        backtrack(1);
        
        
        /* Trier le tableau de solutions */
        qsort(Costs, nb_sol, sizeof(long long), compare_sols); 
        
        
        for (int i=0; i<3; i++) {
        	fscanf(algo[i], "%d %lf %lld %499[^\n] \n", &resultat[i].n, &resultat[i].timeSec, &resultat[i].bestCost, resultat[i].res);
        	res[i].nb_lecture++;
        	res[i].accu += getEcart(resultat[i].bestCost);
    	}
        
        
   }
    
   fprintf(fout, "Voici les résulats finaux: \n");
   
   for (int i=0; i<3; i++) {
   		
   	fprintf(fout, "algo %s a un écart moyen de %.2f\n", names[i], (double) res[i].accu / res[i].nb_lecture);
   		
   }
   	

    fclose(fin);
    fclose(fout);

    return 0;

}
