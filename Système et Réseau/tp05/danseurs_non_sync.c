#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define NB_MENEURS 5
#define NB_SUIVEURS 6

/* attendre delai microsecondes */
void microsleep(int delai);

void *meneur(void *indice);
void *suiveur(void *indice);
void se_preparer();
void suiveur_danser(int num_suiveur, int num_meneur);
void meneur_danser(int num_meneur, int num_suiveur);

int numero_suiveur = -1, numero_meneur = -1;

int main(void)
{
	pthread_t meneurs[NB_MENEURS], suiveurs[NB_SUIVEURS];
	int indices_meneurs[NB_MENEURS], indices_suiveurs[NB_SUIVEURS];
	int i;

	for (i = 0; i < NB_MENEURS; ++i) {
		indices_meneurs[i] = i;
		pthread_create(&meneurs[i], NULL, meneur, &indices_meneurs[i]);
	}
	for (i = 0; i < NB_SUIVEURS; ++i) {
		indices_suiveurs[i] = i;
		pthread_create(&suiveurs[i], NULL, suiveur, &indices_suiveurs[i]);
	}

	for (i = 0; i < NB_MENEURS; ++i)
		pthread_join(meneurs[i], NULL);
	for (i = 0; i < NB_SUIVEURS; ++i)
		pthread_join(suiveurs[i], NULL);
	return 0;
}
void se_preparer()
{
	microsleep(500000 + 400000 * (rand() % 10));
}
void *meneur(void *indice)
{
	int mon_indice = *((int *) indice);
	int indice_partenaire;
	for (;;) {
		se_preparer();
		numero_meneur = mon_indice;
		indice_partenaire = numero_suiveur;
		meneur_danser(mon_indice, indice_partenaire);
	}
	return NULL;
}
void *suiveur(void *indice)
{
	int mon_indice = *((int *) indice);
	int indice_partenaire;
	for (;;) {
		se_preparer();
		numero_suiveur = mon_indice;
		indice_partenaire = numero_meneur;
		suiveur_danser(mon_indice, indice_partenaire);
	}
	return NULL;
}

void suiveur_danser(int num_suiveur, int num_meneur)
{
	printf("suiveur %d danse avec meneur %d\n", num_suiveur, num_meneur);
}
void meneur_danser(int num_meneur, int num_suiveur)
{
	printf("meneur %d danse avec suiveur %d\n", num_meneur, num_suiveur);
}

void microsleep(int delai)
{
	struct timespec ts = { .tv_sec = delai / 1000000,
		.tv_nsec = 1000 * (delai % 1000000)  };
	nanosleep(&ts, NULL);
}
