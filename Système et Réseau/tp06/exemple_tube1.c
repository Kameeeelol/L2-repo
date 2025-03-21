#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

/* lire des caractères sur fdin et les réécrire
 * en majuscule dans fdout
 * se termine lorsque la lecture de fdin est terminée */
void lire_ecrire_majuscule(int fdin, int fdout);

#define BUFSZ 1024
int main(void)
{
	int tube[2]; // tableau de 2 file descriptors
	if (pipe(tube) < 0) { // échec de l'appel système de pipe
		perror("pipe");
		exit(1);
	}
	if (fork() == 0) {
		close(tube[1]); /* fermer l'extrémité du tube non utilisée */
		/* tube[0] : la partie du tube où on fait des lectures
		 * Moyen mnémotechnique : comme 0, l'entrée standard. */
		lire_ecrire_majuscule(tube[0], 1);
		close(tube[0]);
		return 0;
	}
	close(tube[0]); /* fermer l'extrémité du tube non utilisée */
	ssize_t n;
	char buffer[BUFSZ];
	/* lire des caractères sur le terminal jusqu'à la fin du fichier */
	while ((n = read(0, buffer, BUFSZ)) > 0) {
		/* tube[1] : la partie du tube où on fait des écritures
		 * Moyen mnémotechnique : comme 1, la sortie standard. */
		write(tube[1], buffer, n);
	}
	close(tube[1]);
	wait(NULL);
	return 0;
}

void lire_ecrire_majuscule(int fdin, int fdout)
{
	char c;
	ssize_t n;
	char buffer[BUFSZ];
	while ((n = read(fdin, buffer, BUFSZ)) > 0) {
		for (size_t i = 0; i < n; i++) {
			buffer[i] = toupper(buffer[i]);
		}
		write(fdout, buffer, n);
	}
}
