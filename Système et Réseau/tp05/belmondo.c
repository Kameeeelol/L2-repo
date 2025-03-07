#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

struct data {
	int i;
	pthread_mutex_t verrou_i;
	pthread_mutex_t verrou_affichage;
	char **texte;
	int *durees;
};

void *de_funes(void *arg);
void *belmondo(void *arg);

/* attendre delai microsecondes */
void microsleep(int delai);

int main(void)
{
	char *texte[] = {
		"je te tiens", "tu me tiens", "par la barbichette",
		"le premier", "de nous deux", "qui rira", "aura une",
		"ta", "pette", NULL, NULL };
	int durees[] = { 2, 2, 7, 1, 3, 2, 2, 1, 1, 0 };

	struct data donnees = { .i = 0, .texte = texte, .durees = durees };
	
	pthread_t acteur1, acteur2;
	
	pthread_mutex_init(&donnees.verrou_i, NULL);
	pthread_mutex_init(&donnees.verrou_affichage, NULL);
	
	/* rendre la sortie standard unbuffered */
	setvbuf(stdout, NULL, _IONBF, 0);

	if (pthread_create(&acteur1, NULL, de_funes, &donnees) != 0) {
		perror("pthread");
		return 1;
	}
	if (pthread_create(&acteur2, NULL, belmondo, &donnees) != 0) {
		perror("pthread");
		return 1;
	}
	pthread_join(acteur1, NULL);
	pthread_join(acteur2, NULL);
	pthread_mutex_destroy(&donnees.verrou_affichage);
	pthread_mutex_destroy(&donnees.verrou_i);

	return 0;
}

void *de_funes(void *arg)
{
	char *tirade = "";
	struct data *data = arg;
	char a_lire[100];
	int i, delai;
	while (tirade != NULL) {
		/* accès au texte d'origine */
		tirade = data->texte[data->i];
		if (tirade == NULL) {
			break;
		}
		delai = data->durees[data->i] * .5e6 / strlen(tirade);

		/* préparation du texte à lire */
		for (i = 0; tirade[i] != 0; ++i) {
			a_lire[i] = tirade[i] + 
				(tirade[i] == ' ' ? 0 : 'A' - 'a');
			microsleep(delai);
		}
		a_lire[i] = 0;
		
		/* incrémentation du compteur de lignes traitées */
		pthread_mutex_lock(&data->verrou_i);
		data->i = data->i + 1 ;
		pthread_mutex_unlock(&data->verrou_i);
		
		/* lecture du texte */
		pthread_mutex_lock(&data->verrou_affichage);
		for (i = 0; a_lire[i] != 0; ++i) {
			putchar(a_lire[i]);
			microsleep(delai);
		}
		putchar('\n');
		pthread_mutex_unlock(&data->verrou_affichage);
	}
	return NULL;
}

void *belmondo(void *arg)
{
	char *tirade = "";
	struct data *data = arg;
	char a_lire[100];
	int i, delai;
	
	while (tirade != NULL) {
		/* accès au texte d'origine */
		tirade = data->texte[data->i];
		if (tirade == NULL) {
			break;
		}
		delai = data->durees[data->i] * .5e6 / strlen(tirade);
				
		/* préparation du texte à lire */
		for (i = 0; tirade[i] != 0; ++i) {
			a_lire[2 * i] = a_lire[2 * i + 1] = tirade[i];
			microsleep(delai);
		}
		a_lire[2 * i] = 0;

		
		/* incrémentation du compteur de lignes traitées */
		pthread_mutex_lock(&data->verrou_i);
		data->i = data->i + 1 ;
		pthread_mutex_unlock(&data->verrou_i);

		/* lecture du texte */
		pthread_mutex_lock(&data->verrou_affichage);
		for (i = 0; a_lire[i] != 0; ++i) {
			putchar(a_lire[i]);
			microsleep(delai);
		}
		putchar('\n');
		pthread_mutex_unlock(&data->verrou_affichage);
		}	
	return NULL;
}

void microsleep(int delai)
{
	struct timespec ts = { .tv_sec = delai / 1000000,
		.tv_nsec = 1000 * (delai % 1000000)  };
	nanosleep(&ts, NULL);
}
