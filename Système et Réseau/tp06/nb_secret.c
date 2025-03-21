#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000

int main(void)
{
	int nb_secret, nb_util = -1;
	/* On initialise le générateur de nombres aléatoires avec une "graine"
	 * (seed) égale au nombre de secondes depuis le 1er janvier 1970 00:00 */
	srand(time(NULL)); /* à faire une seule fois dans le programme */
	nb_secret = rand() % MAX;
	while (nb_util != nb_secret) {
		scanf("%d", &nb_util);
		if (nb_util == nb_secret) {
			printf("=\n");
		}
		if (nb_util < nb_secret) {
			printf(">\n");
		}
		if (nb_util > nb_secret) {
			printf("<\n");
		}
	}
	return 0;
}
