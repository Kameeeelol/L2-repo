/* fichiers de la bibliothèque standard */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
/* bibliothèque standard unix */
#include <unistd.h> /* close, read, write */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/* spécifique à internet */
#include <arpa/inet.h> /* inet_pton */

#define PORT_INCP 1234

int main(void)
{
	/* 1. Création d'une socket udp ipv4 */
	int sock_l = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_l < 0) {
		perror("socket");
		exit(2);
	}

	/* 2. Création de la sockaddr locale */
	struct sockaddr_in sa = { .sin_family = AF_INET, /* IPv4 */
				  /* Port : 1234 converti si besoin en
				   * gros-boutiste */
				  .sin_port = htons(PORT_INCP),
				  /* Adresse : n'importe laquelle sur laquelle
				   * on reçoit de l'UDP sur port 1234
				   * (en fait c'est 0.0.0.0), convertie si
				   * besoin en gros-boutiste (même si ici ça
				   * ne change rien). */
				  .sin_addr.s_addr = htonl(INADDR_ANY) };
	/* Optionnel : faire en sorte de pouvoir réutiliser l'adresse et le port
	 * sans attendre après la fin du serveur. */
	int opt = 1;
	setsockopt(sock_l, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

	/* 3. Attacher la socket à la sockaddr locale */
	if (bind(sock_l, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		perror("bind");
		exit(3);
	}

	/* 3bis. Configurer sock_l comme socket d'écoute TCP */
	if (listen(sock_l, 128) < 0) {
		perror("listen");
		exit(1);
	}
	for (;;) {
		/* 3ter. Accepter une demande de connexion.
		 * Retourne une nouvelle socket pour les échanges avec le client,
		 * et seulement celui-ci. */
		int s = accept(sock_l, NULL, NULL);
		if (s < 0) {
			perror("accept");
		}

		/* 4. Réception d'une requête INCP */
		/* Il faut absolument une sockaddr pour enregistrer son adresse IP et
		 * son port */

		uint32_t n;
		/* Attendre la réception d'un message d'au plus 4 octets dans s
		 * mettre ces octets dans n */
		ssize_t l = read(s, &n, 4);
		if (l < 4) {
			perror("recvfrom");
			return 2;
		}

		/* 5. Traitement de la requête (protocole applicatif) */
		n = ntohl(n); /* si besoin convertir en boutisme machine */
		n = n + 1; /* incrémentation ! */
		n = htonl(n); /* si besoin, remettre en gros-boutisme */
		/* 6. Envoi de la réponse au client */
		l = write(s, &n, 4);

		if (l < 4) {
			perror("sendto");
			return 3;
		}

		close(s);
	}
	close(sock_l);
	return 0;
}
