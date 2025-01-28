#include "liste.h"
#include <stdio.h>
#include <stdlib.h>

/* allocation sur le tas pour un maillon de valeur v */
/* et renvoi de l'adresse du maillon */
struct maillon *nouveau_maillon (item v)
{
  struct maillon *m = malloc(sizeof(struct maillon));
  m->valeur = v;
  m->suivant = NULL;
  return m;
}

/* libération de l'espace mémoire occupé par le maillon */
void detruire_maillon (struct maillon *m)
{
  destroy(m->valeur);
  free(m);
}

/* allocation sur le tas pour une liste vide */
/* et renvoi de l'adresse de la liste */
struct liste *nouvelle_liste()
{
  struct liste *l = malloc(sizeof(struct liste));
  l->premier = NULL;
  l->longueur = 0;
  return l;
}

/* affichage des valeurs de la liste d'adresse l */
void afficher_liste(const struct liste *l)
{
  struct maillon *m;
  for (m = l->premier; m != NULL; m = m->suivant)
  {
    display(m->valeur);
  }
  printf("\n");
}

/* renvoi de l'adresse du maillon qui occupe la position pos */
/* dans la liste d'adresse l */
struct maillon *acceder_pos_liste(const struct liste *l, unsigned pos)
{
  struct maillon *m;
  for (m = l->premier; m != NULL && pos != 0; pos--)
    m = m->suivant;
  return m;
}

/* insertion du maillon d'adresse m au début de la liste d'adresse l */
void ajouter_maillon_debut_liste(struct liste *l, struct maillon *m)
{
  m->suivant = l->premier;
  l->premier = m;
  ++l->longueur;
}

/* insertion d'un maillon d'adresse m à la fin de la liste d'adresse l */
void ajouter_maillon_fin_liste(struct liste *l, struct maillon *m)
{
  struct maillon *p;
  if (l->longueur == 0) { ajouter_maillon_debut_liste(l,m); return; }
  p = acceder_pos_liste(l, l->longueur - 1);
  p->suivant = m;
  m->suivant = NULL;
  ++l->longueur;
}

/* extraction du premier maillon de la liste d'adresse l */
/* et renvoi de l'adresse du maillon extrait */
struct maillon *extraire_maillon_debut_liste(struct liste *l)
{
  struct maillon *m = l->premier;
  if (m != NULL)
  {
    l->premier = m->suivant;
    --l->longueur;
    m->suivant = NULL;
  }
  return m;
}

/* libération de l'espace occupé par la liste et tous ses maillons */
void detruire_liste (struct liste **l)
{
  while ((*l)->longueur > 0) {  detruire_maillon(extraire_maillon_debut_liste(*l)); }
  free(*l);
  *l = NULL;
}
