#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HAUTEUR 40
#define LARGEUR 80
#define NUM_ITER_DFT 30
#define DELAI 250000

int ecran[HAUTEUR][LARGEUR];
void effacer_lignes(int num_lignes);

/* Attendre delai microsecondes */
void microsleep(int delai);

int main(int argc, char *argv[])
{
	int num_iter = argc > 1 ? atoi(argv[1]) : NUM_ITER_DFT;
	int n, i, j;
	for (n = 0; n < num_iter; ++n) {
		/* calcul de l'affichage suivant */
		microsleep(DELAI); /* simuler un calcul long */
		for (i = HAUTEUR - 1; i > 0; --i)
			for (j = 0; j < LARGEUR; ++j)
				ecran[i][j] = ecran[i - 1][j];
		for (j = 0; j < LARGEUR; ++j)
			ecran[0][j] = 0;
		ecran[0][rand() % LARGEUR] = 1;

		/* affichage de l'écran modifié */
		microsleep(DELAI); /* simuler un affichage qui prend du temps */
		effacer_lignes(HAUTEUR);

		for (i = 0; i < HAUTEUR; ++i) {
			for (j = 0; j < LARGEUR; ++j) {
				if (ecran[i][j] == 0)
					putchar(' ');
				else
					putchar('*');
			}
			putchar('\n');
		}
	}
	puts("YOU LOSE.\n");
	return 0;
}

void effacer_lignes(int num_lignes)
{
	printf("\033[2K");
	for(; num_lignes > 0; --num_lignes) {
		printf("\033[F");
		printf("\033[2K");
	}
}

void microsleep(int delai)
{
	struct timespec ts = { .tv_sec = delai / 1000000,
		.tv_nsec = 1000 * (delai % 1000000)  };
	nanosleep(&ts, NULL);
}
