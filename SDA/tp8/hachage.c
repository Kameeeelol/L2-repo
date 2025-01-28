#include "liste.h"
#include <stdio.h> /* Pour le type FILE et les fonctions fopen, fclose, feof, fscanf, DONT VOUS N'AVEZ PAS À VOUS PRÉOCCUPER */
#include <stdlib.h> /* Pour NULL, EXIT_SUCCESS */
#include <string.h>

/* CM: Pour le chainage séparé Choix typique : m ∼ n/4 ... Et DataFile contient 109 mots*/
#define N 28 /* À COMPLÉTER par la taille de la table de hachage */

#define ENTREE "DataFile"/* À COMPLÉTER par le nom du fichier dont le programme va "hacher" les mots */

void insert_string(char *, struct liste **); /* insère un mot dans la table de hachage */
void usage_hachage(char *);
unsigned hash(char *); /* calcule l'empreinte (i.e. le haschcode) d'un mot */

int main(int argc, char **argv) {

        /* LES DEUX VARIABLES DÉCLARÉES CI-DESSOUS ne servent que pour "extraire" les mots du fichier ENTREE */
        /* VOUS N'AVEZ PAS À VOUS PRÉOCCUPER DE CES VARIABLES */
        FILE *flot;
        char mot[30]; /* on suppose que les mots du fichier ont au plus 30 lettres */
        int i, nb_colisions=0;

        struct liste * tab[N];

        if (argc != 2) {
                printf("Nombre d'arguments erroné. Recommencez ! \n");
                usage_hachage(argv[0]);
                return EXIT_FAILURE;
        }

        for (i = 0; i < N; ++i) {
                tab[i] = nouvelle_liste();
        }

        /** Lecture mot à mot du fichier ENTREE */
        /* VOUS N'AVEZ PAS À VOUS PRÉOCCUPER DE CETTE MISE À JOUR (sauf pour la définition de la fonction hacher) */
        if ((flot = fopen(ENTREE, "r")) != NULL) {
                do {
                        /* On ne comptabilise pas les mots qui commencent par autre chose qu'une lettre de l'alphabet latin sans signe diacritique ni accent */
                        /* On suppose que chaque a moins de 30 lettres et on supprime l'éventuel signe de ponctuation qui le conclut */
                        if (fscanf(flot, "%s", mot) == 1 && ((mot[0] >= 'A' && mot[0] <= 'Z') || (mot[0] >= 'a' && mot[0] <= 'z'))) {
                                int l = strlen(mot) - 1;
                                if ((mot[l] < 'A' || mot[l] > 'Z') && (mot[l] < 'a' || mot[l] > 'z')) {
                                        mot[l] = '\0';
                                }
                                insert_string(mot, tab);
                        }
                } while (!feof(flot));
                fclose(flot);
        }
        /** FIN de la lecture mot à mot du fichier ENTREE */
        
        
        /* Affichage de la table de hachage et comptage des collisions */
        for (i = 0; i < N; ++i) {
        	afficher_liste(tab[i]);
                if (tab[i]->longueur > 0) 
                        nb_colisions += tab[i]->longueur - 1;
                
        }

        printf("Au total, il y a eu %d collisions.\n", nb_colisions);

        /* Nettoyage : libération de la mémoire */
        for (i = 0; i < N; ++i) {
                detruire_liste(&tab[i]);
        }

	
	
        return EXIT_SUCCESS;
}

/** Affiche le "format" de la ligne de commande en cas d'erreur */
void usage_hachage(char *s) {
        printf("%s string \n", s);
        printf("argument : nom du fichier de lecture \n");
}

/** Fonction de hachage djb2 due à Dan Bernstein */
unsigned hash(char *str) {
        unsigned long hash = 5381;
        int c;

        while (c == *str++) {
                hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }

        return hash % N;
}

/** Insère le mot dans la table de hachage avec chaînage séparé */
void insert_string(char *s, struct liste * tab[]) {
        int i = hash(s);
        char * chaine = malloc(strlen(s));
        strcpy(chaine, s);
	ajouter_maillon_debut_liste(tab[i], nouveau_maillon(chaine));
}