#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "question.h"
#include "sys_utils.h"
#include <inttypes.h>

struct question *creer_question(const char *enonce)
{
	struct question *q = malloc_or_exit(sizeof(struct question));
	q->enonce = malloc_or_exit(strlen(enonce) + 1);
	q->cap_props = CAP_DFT; 
	q->propositions = malloc_or_exit(q->cap_props * sizeof(char *));
	q->nb_props = 0;
	strcpy(q->enonce, enonce);
	return q;
}

void ajouter_prop(struct question *q, const char *prop)
{
	char *nouv_prop = malloc_or_exit(strlen(prop) + 1);
	strcpy(nouv_prop, prop);
	q->nb_props++;
	if (q->cap_props < q->nb_props) {
		q->cap_props *= 2;
		q->propositions = realloc(q->propositions, q->cap_props * sizeof(char *));
		if (q->propositions == NULL)
			perror_exit("realloc");
	}
	q->propositions[q->nb_props - 1] = nouv_prop;
}

void detruire_question(struct question *q)
{
	if (q == NULL)
		return;
	for (uint16_t i = 0; i < q->nb_props; ++i)
		free(q->propositions[i]);
	free(q->propositions);
	free(q);
}

struct banque_questions *init_banque_questions(const char *fichier_questions)
{
	FILE *f = fopen(fichier_questions, "r");
	if (f == NULL) {
		perror("fopen");
		exit(2);
	}
	struct banque_questions *res = malloc_or_exit(sizeof(*res));
	res->nb_questions = 0;
	char ligne[L_LIGNE];
	int i = 0;
	/* Premier parcours pour le nombre de questions */
	while (fgets(ligne, L_LIGNE - 1, f) != NULL) {
		*strchr(ligne, '\n') = '\0';
		if (strcmp(ligne, "question") == 0)
			res->nb_questions += 1;
	}
	res->tab = malloc_or_exit(res->nb_questions * sizeof(struct question *));
	rewind(f);

	int etat; /* 0 pour en attente des réponses
		     1 pour en attente de la question
		     2 pour en attente de la bonne réponse */
	/* 2ème parcours pour remplir les questions */
	while (fgets(ligne, L_LIGNE - 1, f) != NULL) {
		*strchr(ligne, '\n') = '\0';
		if (strcmp(ligne, "question") == 0) {
			etat = 1;
		} else if (etat == 1) {
			res->tab[i] = creer_question(ligne);
			etat = 0;
		} else if (strcmp(ligne, "reponse") == 0) {
			etat = 2;
		} else if (etat == 2) {
			sscanf(ligne, "%" SCNu16, &res->tab[i++]->ind_rep);
			etat = -1;
		} else {
			ajouter_prop(res->tab[i], ligne);
		}
	}
	fclose(f);
	srand(time(NULL));
	return res;
}

struct question *question_aleatoire(const struct banque_questions *bq)
{
	return bq->tab[rand() % bq->nb_questions];
}

void detruire_banque_questions(struct banque_questions *bq)
{
	for (int i = 0; i < bq->nb_questions; ++i)
		detruire_question(bq->tab[i]);
	free(bq->tab);
	free(bq);
}
