/* fichiers de la bibliothèque standard */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* bibliothèque standard unix */
#include <unistd.h> /* close, read, write */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/* spécifique à internet */
#include <arpa/inet.h> /* inet_pton */
/* threads */
#include <pthread.h>

#include "sys_utils.h"
#include "question.h"

#define PORT_QUIZZP 6666
#define FICH_QUESTIONS "questionnaire.txt"

/* Pour pouvoir partager les tâches entre différents threads (en fin d'énoncé) */
struct work {
	int socket;
	struct question *q;
};

/** Créer et configure une socket d'écoute TCP sur IPv4 sur le port associé au
 * Quizz Protocol. Retourne le descripteur de fichier de la socket ainsi créée
 * en cas de succès, met fin au programme sinon. */
int init_sock_ecoute();

/** Accepter les demandes de connection entrantes sur la socket d'écoute
 * sock.
 * Retourne le descripteur de fichier de la socket d'échange créée ou -1 en cas
 * d'erreur */
int connection_au_client(int sock);

/** Recevoir l'adresse d'une variable de type struct work convenablement
 * initialisée, puis échanger avec un client en utilisant le champ socket de
 * cette structure pour la question (champ q) de cette strucure.
 * Retourne : toujours NULL. */
void *worker(void *work);

/* Mener tous les échanges avec le client connecté à la socket fd, ainsi que 
 * spécifié dans le Quizz Protocol, avec la question d'adresse q */
void echanger_avec_client(int fd, const struct question *q);
void echanger_avec_client_test(int fd, const struct question *q);

int main(void)
{
	struct banque_questions *bq = init_banque_questions(FICH_QUESTIONS);
	int sock = init_sock_ecoute();
#define MONO 1
	for (;;) {
		/* Version 1 sans threads */
		int se = connection_au_client(sock);
		if (se < 0)
			continue;
		echanger_avec_client(se, question_aleatoire(bq));
		close(se);
		/* Version 2 avec threads */
		/* TODO */
	}
	/* On n'y arrive jamais normalement. */
	close(sock);
	detruire_banque_questions(bq);
	return 0;
}

int init_sock_ecoute()
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if ( sock == -1 ) perror_exit ("sock");
	struct sockaddr_in client_addr;
	memeset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = INADDR_ANY;
	client_addr.sin_port = htons(PORT_QUIZZP);
	if (bind(sock, (struct sockaddr_in *) &client_addr, sizeof(client_addr)) == -1) perror_exit("bind");
	if(listen(sock, 1) == -1) perror_exit("listen");
	return sock;
}

int connection_au_client(int sock)
{
	return 0;
}

void *worker(void *work)
{
	return NULL;
}

void echanger_avec_client(int fd, const struct question *q)
{
}

