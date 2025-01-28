#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 3
#define N_MAX 10

// Définir une structure de stats
typedef struct {
    int nb_echanges;
    int nb_comparaisons;
} Stats;

// Fonction d'échange 
void echange(float *x, float *y){
    float tmp = *x;
    *x = *y;
    *y = tmp;
}

// Fonction pour afficher le tableau
void afficher_tableau(float *tab, int taille){
    for(int i = 0; i < taille; i++){
        printf("%.2f ", tab[i]);
    }
    printf("\n");
}

// Fonction de tri par insertion 
Stats tri_insertion(float *tab, int taille){
    Stats stats_insertion = {0,0};
    int i, j;
    for (i = 1; i < taille; ++i) {
        j = i;
        while (j > 0) {
            stats_insertion.nb_comparaisons++;
            if(tab[j] < tab[j-1]){
                echange(&tab[j], &tab[j-1]);
                stats_insertion.nb_echanges++;
                --j;
            } else {
                break;
            }
        }
    }
    return stats_insertion;
}

// Fonction qui recherche le plus petit élément du tableau 
int recherche_pos_min (float tab[], int taille) {
    int i, pos_min = 0;
    for (i = 1; i < taille; ++i)
        if (tab[i] < tab[pos_min]) { pos_min = i; }
    return pos_min;
}

// Fonction de tri par sélection 
Stats tri_selection(float *tab, int taille){
    Stats stats_selection = {0,0};
    int i, pos_min;
    for (i = 0; i < taille - 1; ++i) {
        pos_min = recherche_pos_min(tab + i, taille - i);
        stats_selection.nb_comparaisons += (taille - i - 1);
        echange(&tab[i], &tab[i + pos_min]);
        stats_selection.nb_echanges++;
    }
    return stats_selection;
}

// fonction de median 
int median(float *tab, int g, int d){
    int mid = (g - (d-g))/2;
    if (tab[g]> tab[mid]) echange(&tab[g],&tab[mid]);
    if (tab[g]> tab[d]) echange(&tab[g],&tab[d]);
    if (tab[mid]> tab[d]) echange(&tab[mid],&tab[d]);
    return mid;    
}

//fonction de partition
int partition(float *tab, int g, int d){
    int mid = median(&tab, g, d);
    int i = g-1;
    float pivot = tab[mid];
    for(int j=g, j<d, j++){
        if (tab[j]<pivot){
            i++;
            echange(&tab[i],&tab[j]);
        }
    }
    echange(&tab[i+1], &tab[d]);
    return i+1;
}

// foncion de tri rapide
void tri_rapide(float *tab, int g, int d){
    if (d<=g) return;
    int i = partition(&tab, g, d);
    tri_rapide()
    
}

// partition 2 

int partitionn(float *tab, int g ,int d){
    int i = g-1;
    int k = d;
    float pivot = tab[d];
    for (;;){
        while(tab[i++]<pivot);
        while(tab[j--])
            if (j==g) break;
        if ( i>=j) break;
        echange(&tab[i],&tab[j])
    }
    echange(&tab[i],&tab[j])
    return i;
}



// Fonction qui initialise le tableau aléatoirement 
void init_tableau_aleatoire(float *tab, int taille){
    for(int i = 0; i < taille; i++){
        tab[i] = (float)(rand() % 1000)/10.0;
    }
}

// Fonction pour initialiser le tableau au clavier 
void init_tableau_clavier(float *tab, int taille){
    for(int i = 0; i < taille; i++){
        printf("Entrez la valeur numéro %d du tableau: ", i+1);
        if(scanf("%f", &tab[i]) != 1){
            printf("Erreur de saisie.\n");
            exit(1);
        }
    }
}

// Fonction qui copie le tableau 
void copier_tableau(float *source, float *destination, int taille){
    for(int i = 0; i < taille; i++){
        destination[i] = source[i];
    }
}

// Le main 
int main(int argc, char *argv[]){
    if(argc < 3){
        printf("Usage: %s <taille> <modeTab>\n", argv[0]);
        return 1;
    }

    int taille = atoi(argv[1]);
    if(taille <= 0){
        printf("La taille doit être un entier positif.\n");
        return 1;
    }

    int modeTab = atoi(argv[2]);
    float *tab[N];

    // Allouer de la mémoire pour chaque tableau
    for(int i = 0; i < N; i++){
        tab[i] = malloc(taille * sizeof(float));
        if(tab[i] == NULL){
            perror("Erreur d'allocation de mémoire");
            // Libérer la mémoire allouée précédemment avant de quitter
            for(int j = 0; j < i; j++) {
                free(tab[j]);
            }
            return 1;
        }
    }

    if (modeTab == 0){
        srand(time(NULL));
        init_tableau_aleatoire(tab[0], taille);
    } else if (modeTab == 1) {
        init_tableau_clavier(tab[0], taille);
    } else{
        printf("Mode d'initialisation invalide, tapez 0 pour une initialisation aléatoire ou 1 pour une initialisation à la main\n");
        // Libérer la mémoire allouée avant de quitter
        for(int i = 0; i < N; i++) {
            free(tab[i]);
        }
        return 1; 
    }

    printf("Le tableau initial :\n");
    afficher_tableau(tab[0], taille);

    copier_tableau(tab[0], tab[1], taille);
    copier_tableau(tab[0], tab[2], taille);

    Stats stats_insertion = tri_insertion(tab[1], taille);
    printf("\nLe tableau après le tri par insertion :\n");
    afficher_tableau(tab[1], taille);
    printf("\nComparaisons : %d, Échanges : %d\n", stats_insertion.nb_comparaisons, stats_insertion.nb_echanges);

    Stats stats_selection = tri_selection(tab[2], taille);
    printf("\nLe tableau après le tri par sélection :\n");
    afficher_tableau(tab[2], taille);
    printf("\nComparaisons : %d, Échanges : %d\n", stats_selection.nb_comparaisons, stats_selection.nb_echanges);

    // Libérer la mémoire allouée
    for(int i = 0; i < N; i++){
        free(tab[i]);
    }

    return 0;
}



// Fonction pour scinder une liste en deux sous-listes de tailles aussi proches que possible
void split_list(list* ll, list** l_1, list** l_2) {
    // Si la liste est vide ou contient un seul élément, elle ne peut pas être scindée
    if (ll == NULL || ll->next == NULL) {
        *l_1 = ll;       // l_1 sera la liste originale
        *l_2 = NULL;     // l_2 sera vide
        return;
    }

    // Pointeurs slow et fast pour trouver le milieu
    list* slow = ll;
    list* fast = ll;
    list* prev = NULL;   // Pointeur pour conserver l'avant-dernier élément de la première moitié

    // Parcourir la liste
    while (fast != NULL && fast->next != NULL) {
        prev = slow;           // Retenir l'élément avant slow
        slow = slow->next;     // Slow avance d'un pas
        fast = fast->next->next;  // Fast avance de deux pas
    }

    // Couper la liste en deux
    *l_1 = ll;                 // l_1 commence à la tête de la liste originale
    *l_2 = slow;               // l_2 commence à slow

    // Terminer la première sous-liste
    if (prev != NULL) {
        prev->next = NULL;
    }
}

void mergesort_list(list **ll) {
    if (*ll == NULL || (*ll)->next == NULL) {
        return;
    }

    list *l_1 = NULL;
    list *l_2 = NULL;

    // Diviser la liste en deux
    split_list(*ll, &l_1, &l_2);

    // Appliquer récursivement le tri sur les sous-listes
    mergesort_list(&l_1);
    mergesort_list(&l_2);

    // Fusionner les deux sous-listes triées
    merge_list(ll, l_1, l_2);
}
