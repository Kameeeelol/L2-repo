#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define N 5
#define COEFF_ATTENTE 100000
#define COEFF_DELTA   5000

/* attendre delai microsecondes */
void microsleep(int delai);

struct pour_compter {
	int i;
};

void *compteur(void *);
void prendre_du_temps_pour_compter(void);

int main(void)
{
	int i;
	pthread_t th[N];
	struct pour_compter p_c[N];

	for (i = 0; i < N; ++i) {
		p_c[i].i = i;
		if (pthread_create(&th[i], NULL, compteur, &p_c[i]) != 0) {
			perror("create");
			return 1;
		}
	}

	for (i = 0; i < N; ++i)
		pthread_join(th[i], NULL);
	return 0;
}

void *compteur(void *arg)
{
	struct pour_compter *pc = arg;
	int i;
	for (i = 0; i <= 100 ; i ++) {
		prendre_du_temps_pour_compter();
		printf("%s%c : %d\n", i % 2 ? "\033[m" : "\033[31m", 'a'+ pc->i, i);
	}
	return NULL;
}

void prendre_du_temps_pour_compter(void)
{
	microsleep(COEFF_ATTENTE + (rand() % 10) * COEFF_DELTA );
}

void microsleep(int delai)
{
	struct timespec ts = { .tv_sec = delai / 1000000,
		.tv_nsec = 1000 * (delai % 1000000)  };
	nanosleep(&ts, NULL);
}
