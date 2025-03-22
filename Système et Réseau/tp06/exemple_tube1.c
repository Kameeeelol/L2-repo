#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

/* lire des caractères sur fdin et les réécrire
 * en majuscule dans fdout
 * se termine lorsque la lecture de fdin est terminée */
void lire_ecrire_majuscule(int fdin, int fdout);
void lire_ecrire_deuxfois(int fdin, int fdout);

#define BUFSZ 1024
int main(void)
{
	int tube[2]; // tableau de 2 file descriptors
	int tube2[2];
	if (pipe(tube) < 0 || pipe(tube2) < 0) { // échec de l'appel système de pipe
		perror("pipe");
		exit(1);
	}
	if (fork() == 0) {
		close(tube[1]); /* fermer l'extrémité du tube non utilisée */
		close(tube2[0]);
		/* tube[0] : la partie du tube où on fait des lectures
		 * Moyen mnémotechnique : comme 0, l'entrée standard. */
		lire_ecrire_majuscule(tube[0], tube2[1]);
		close( tube2[1]);
		close(tube[0]);
		exit(0);
	}

	if ( fork() == 0 ){
		close(tube[0]);
		close(tube[1]);
		close(tube2[1]);
		lire_ecrire_deuxfois(tube2[0], 1);
		close(tube2[0]);
		exit(0);
	}


	close(tube[0]); /* fermer l'extrémité du tube non utilisée */
	close(tube2[1]);
	close(tube2[0]);
	ssize_t n;
	char buffer[BUFSZ];
	/* lire des caractères sur le terminal jusqu'à la fin du fichier */
	while ((n = read(0, buffer, BUFSZ)) > 0) {
		for ( int i=0; i < n; i++){
			if ( isdigit(buffer[i]) ) buffer[i] = ' ';
		}
		/* tube[1] : la partie du tube où on fait des écritures
		 * Moyen mnémotechnique : comme 1, la sortie standard. */
		write(tube[1], buffer, n);
	}
	close(tube[1]);
	wait(NULL);
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

void lire_ecrire_deuxfois(int fdin, int fdout)
{
	char c;
	ssize_t n;
	char buffer[BUFSZ];
	while ((n = read(fdin, buffer, BUFSZ)) > 0) {
		write(fdout, buffer, n);
		write(fdout, buffer, n);
	}
}
