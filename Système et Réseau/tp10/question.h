#ifndef _QUESTION_H
#define _QUESTION_H
#include <stdint.h>
#define CAP_DFT 10

#define L_LIGNE 256 /* Longueur maximale d'une ligne dans une question */

struct question {
	char *enonce;
	char **propositions; /* tableau des propositions */
	uint16_t nb_props; /* nombre de propositions */
	uint16_t cap_props; /* capacité du tableau propositions */
	uint16_t ind_rep; /* indice de la bonne réponse dans le tableau */
};

/** Retourner un pointeur vers une nouvelle question d'énoncé égal à enonce
 * de cap_props égal à CAP_DFT, ou termine le programme en cas d'erreur
 * d'allocation de mémoire. */
struct question *creer_question(const char *enonce);

/** Ajouter (à la fin) la proposition prop aux propositions de la question
 * d'adresse q */
void ajouter_prop(struct question *q, const char *prop);

/** Rendre le champ ind_rep de la question d'adresse q égal à ind_rep */
void set_ind_rep(struct question *q, uint16_t ind_rep);

/** Libèrer toute la mémoire allouée à la question q */
void detruire_question(struct question *q);

struct banque_questions {
	struct question **tab;
	int nb_questions;
};

/** Créer et initialiser une banque de question avec les questions du fichier
 * fichier_question.
 * Retourne l'adresse de la banque de questions ainsi créée (en cas d'erreur,
 * met fin au programme). */
struct banque_questions *init_banque_questions(const char *fichier_questions);

/** Retourner une question choisie uniformément au hasard dans la banque de
 * questions d'adresse bq */
struct question *question_aleatoire(const struct banque_questions *bq);

/** Libèrer toute la mémoire associée à la banque de questions d'adresse bq */
void detruire_banque_questions(struct banque_questions *bq);

#endif /* ifndef _QUESTION_H */
