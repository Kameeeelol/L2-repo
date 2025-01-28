#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 5
#define N_MAX 10
float aux[N_MAX];


// definir une structure de stats
typedef struct {
    int nb_affectations;
    int nb_comparaisons;
}Stats;

// fonction d'echange 
void echange(float *x, float *y, Stats *s){
    float tmp = *x;
    *x = *y;
    *y = tmp;
    s->nb_affectations += 3;
}
 
// fonction pour afficher le tableau
void afficher_tableau(float *tab, int taille){
    for(int i=0; i<taille; i++){
        printf("%.2f  ",tab[i]);
    }
    printf("\n");
}

// fonction de tri par insertion 
Stats tri_insertion(float *tab, int taille){
    Stats stats_insertion = {0,0};
    int i, j;
    for (i = 1; i < taille; ++i) {
        j = i;
        while (j > 0){ 
            stats_insertion.nb_comparaisons++;   
            if (tab[j] < tab[j-1]) { 
            echange (&tab[j], &tab[j - 1],&stats_insertion);
            --j;
            }else{
                break;
            }
        }
  }
  return stats_insertion;
}

// fonction qui recherche le plus petit elements du tableau 
int recherche_pos_min (float tab[], int taille) {
  int i, pos_min = 0;
  for (i = 1; i < taille; ++i)
    if (tab[i] < tab[pos_min]) { pos_min = i; }
  return pos_min;
}


// fonction de tri par selection 
Stats tri_selection(float *tab, int taille){
      Stats stats_selection = {0,0};
      int i, pos_min;
      for (i = 0; i < taille - 1; ++i) {
        pos_min = recherche_pos_min (tab + i, taille - i);
        stats_selection.nb_comparaisons += (taille - i - 1);
        echange (&tab[i], &tab[i + pos_min],&stats_selection);
    }
    return stats_selection;
}

// fonction de tri par fusion 
void fusion (float tab[], int g, int m, int d, Stats *s) {
	int i, j, k;
    for (i = m + 1; i > g; i--){
        aux[i - 1] = tab[i - 1];
        s->nb_affectations++;
    }
	for (j = m; j < d; j++){
        aux[d + m - j] = tab[j + 1];
        s->nb_affectations++;
    }
	/* on a recopie en renversant l'ordre des cles */
	for (k = g; k <= d; k++){
		if (aux[i] < aux[j]){
			s->nb_comparaisons++;
            tab[k] = aux[i++]; /* ce qui revient a : {tab[k] = aux[i]; i++;} */
            s->nb_affectations++;
        }else{
			s->nb_comparaisons++;
            tab[k] = aux[j--]; /* ce qui revient a : {tab[k] = aux[j]; j--;} */
            s->nb_affectations++;
        }
    }
}

// fonction de tri fusion 
void tri_fusion (float *tab, int g, int d, Stats *p) {
    int m = (g + d)/2;
	if (d <= g)
		return;
	tri_fusion (tab, g, m, p);
	tri_fusion (tab, m + 1, d, p);
	fusion (tab, g, m, d,p);
}

// fonction de partition
int partition(float tab[], int g, int d, Stats *s) {
    int i = g-1, j = d; float pivot = tab[d];
	for (;;) {
		s->nb_comparaisons++;
        while (tab[++i] < pivot) ;
		s->nb_comparaisons++;
        while (pivot < tab[--j])
            if (j == g) break;
		if (i >= j) break;
		echange(&tab[i], &tab[j], s);
	}
	echange(&tab[i], &tab[d], s);
	return i;
}


// fonction de tri rapide
void tri_rapide(float tab[], int g, int d, Stats *s) {
    int i;
	if (d <= g) return;
	i = partition(tab, g, d, s);
	tri_rapide(tab, g, i-1, s);
	tri_rapide(tab, i+1, d, s);
    

}


// fonction qui initialise le tableau aleatoirement 
void init_tableau_aleatoire(float *tab, int taille){
    for(int i=0; i<taille; i++){
        tab[i] = (float)(rand() % 1000)/10.0;
    }
}

// fonction pour initialisé le tableau au clavier 
void init_tableau_clavier(float *tab, int taille){
    for(int i=0; i<taille; i++){
        printf("Entrez la valeur numero %d du tableau",i+1);
        scanf("%f", &tab[i]);
    }
}

//fonction qui copie le tableau 
void copier_tableau(float *source, float *destination, int taille){
    for(int i=0; i<taille; i++){
        destination[i] = source[i];
    }
}

// le main 

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("il nous faut exactement deux argument");
        return 1;
    }
    
    int taille = atoi(argv[1]);
    int modeTab = atoi(argv[2]);
    float *tab[N]; 
    for(int i = 0; i < N; i++){
        tab[i] = malloc(taille * sizeof(float));
        if(tab[i] == NULL){
            for(int j = 0; j < i; j++) {
                free(tab[j]);
            }
            return 1;
        }
    }
    if (modeTab==0){
        srand(time(NULL));
        init_tableau_aleatoire(tab[0], taille);
    } else if (modeTab==1) {
        init_tableau_clavier(tab[0],taille);
    }else{
        printf("Mode d'initialisation invalide, tapez 0 pour une initialisation aleatoire ou 1 pour une initialisation a la main\n");
        return 1; 
    }
    printf("le tableau initiale :\n");
    afficher_tableau(tab[0],taille);
    copier_tableau(tab[0],tab[1],taille);
    copier_tableau(tab[0],tab[2],taille);
    copier_tableau(tab[0],tab[3],taille);
    copier_tableau(tab[0],tab[4],taille);
    Stats stats_insertion = tri_insertion (tab[1], taille);
    printf("\nle tableau après le tri par insertion :\n");
    afficher_tableau(tab[1],taille);
    printf("\ncomparaison : %d, affectation : %d\n", stats_insertion.nb_comparaisons, stats_insertion.nb_affectations);
    Stats stats_selection = tri_selection (tab[2], taille);
    printf("\nle tableau après le tri par selection :\n");
    afficher_tableau(tab[2],taille);
    printf("\ncomparaison : %d, affectations : %d\n", stats_selection.nb_comparaisons, stats_selection.nb_affectations);
    Stats stats_fusion = {0,0};
    tri_fusion (tab[3], 0, (taille-1), &stats_fusion);
    printf("\nle tableau après le tri fusion :\n");
    afficher_tableau(tab[3],taille);
    printf("\ncomparaison : %d, affectation : %d\n", stats_fusion.nb_comparaisons, stats_fusion.nb_affectations);
    Stats stats_rapide = {0,0};
    tri_rapide (tab[4], 0, (taille-1), &stats_rapide);
    printf("\nle tableau après le tri rapide :\n");
    afficher_tableau(tab[4],taille);
    printf("\ncomparaison : %d, affectation : %d\n", stats_rapide.nb_comparaisons, stats_rapide.nb_affectations);
    for(int i = 0; i < N; i++){
        free(tab[i]);
    }
    return 0;
    
}