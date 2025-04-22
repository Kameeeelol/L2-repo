/* fichiers de la bibliothèque standard */
#include <stdio.h>
#include <stdlib.h>
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
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(2);
	}

	/* 2. Création de la sockaddr locale */
	struct sockaddr_in sa = {
		.sin_family = AF_INET, /* IPv4 */
		/* Port : 1234 converti si besoin en gros-boutiste */
		.sin_port = htons(PORT_INCP),
		/* Adresse : n'importe laquelle sur laquelle on reçoit de l'UDP
		 * sur port 1234 (en fait c'est 0.0.0.0), convertie si besoin
		 * en gros-boutiste (même si ici ça ne change rien). */
		.sin_addr.s_addr = htonl(INADDR_ANY) };

	/* 3. Attacher la socket à la sockaddr locale */
	if (bind(sock, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		perror("bind");
		exit(3);
	}

	/* 4. Réception d'une requête INCP */
	/* Il faut absolument une sockaddr pour enregistrer son adresse IP et
	 * son port */
	struct sockaddr_in addr_clt;
	socklen_t sl = sizeof(struct sockaddr_in);

	uint32_t n;
	/* Attendre la réception d'un message d'au plus 4 octets dans sock
	 * mettre ces octets dans n
	 * sans flag (0)
	 * mettre l'adresse et le port de l'expéditeur dans les *sl premiers
	 * octets de addr_clt
	 * et mettre dans sl la "vraie" taille de l'adresse du client (on
	 * croise les doigts pour que ce soit bien celle qui était prévue...) */
	ssize_t s = recvfrom(sock, &n, 4, 0, (struct sockaddr *) &addr_clt, &sl);
	if (s < 4) {
		perror("recvfrom");
		return 2;
	}

	/* 5. Traitement de la requête (protocole applicatif) */
	n = ntohl(n); /* si besoin convertir en boutisme machine */
	n = n + 1; /* incrémentation ! */
	n = htonl(n); /* si besoin, remettre en gros-boutisme */
	/* 6. Envoi de la réponse au client */
	s = sendto(sock, &n, 4, 0, (struct sockaddr *) &addr_clt, sl);

	if (s < 4) {
		perror("sendto");
		return 3;
	}

	close(sock);
	return 0;
}
