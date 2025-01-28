#ifndef _LISTE_H
#define _LISTE_H

#include "item.h"

struct maillon {
  item valeur; /* valeur du maillon */
  struct maillon *suivant; /* adresse du maillon suivant */
};

struct liste {
  struct maillon *premier; /* adresse du premier maillon de la liste */
  int longueur; /* nombre de maillons dans la liste */
};

/* allocation sur le tas pour un maillon de valeur v */
/* et renvoi de l'adresse du maillon */
struct maillon *nouveau_maillon (item v);

/* libération de l'espace mémoire occupé par le maillon */
void detruire_maillon (struct maillon *m);

/* allocation sur le tas pour une liste vide */
/* et renvoi de l'adresse de la liste */
struct liste *nouvelle_liste();

/* affichage des valeurs de la liste d'adresse l */
void afficher_liste(const struct liste *l);

/* renvoi de l'adresse du maillon qui occupe la position pos */
/* dans la liste d'adresse l */
struct maillon *acceder_pos_liste(const struct liste *l, unsigned pos);

/* insertion du maillon d'adresse m au début de la liste d'adresse l */
void ajouter_maillon_debut_liste(struct liste *l, struct maillon *m);

/* insertion d'un maillon d'adresse m à la fin de la liste d'adresse l */
void ajouter_maillon_fin_liste(struct liste *l, struct maillon *m);

/* extraction du premier maillon de la liste d'adresse l */
/* et renvoi de l'adresse du maillon extrait */
struct maillon *extraire_maillon_debut_liste(struct liste *l);

/* libération de l'espace occupé par la liste et tous ses maillons */
void detruire_liste (struct liste **l);

#endif
