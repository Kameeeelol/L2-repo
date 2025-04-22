/* fichiers de la bibliothèque standard */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
/* bibliothèque standard unix */
#include <unistd.h> /* close, read, write */
#include <sys/types.h>
#include <sys/socket.h>
/* spécifique à internet */
#include <arpa/inet.h> /* inet_pton */

#define PORT_INCP 1234

void usage(char *nom_prog)
{
	fprintf(stderr, "Usage: %s addr_ipv4\n"
			"client pour INCP (Incrementation Protocol)\n"
			"Exemple: %s 208.97.177.124\n", nom_prog, nom_prog);
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}
	/* 1. Création d'une socket ipv4 udp */
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(2);
	}

	/* 2. Création de la sockaddr distante */
	struct sockaddr_in sa = { .sin_family = AF_INET, /* IPv4 */
				  /* hotns : Host To Network Short pour
				   * convertir si besoin un entier sur 16 bits
				   * en gros-boutiste */
				  .sin_port = htons(PORT_INCP)
				};

	/* "Internet Presentation to Numeric" convertit une représentation
	 * d'adresse IP (v4 ou v6) en numérique (gros-boutiste) */
	if (inet_pton(AF_INET, argv[1], &sa.sin_addr) != 1) {
		fprintf(stderr, "adresse ipv4 non valable\n");
		exit(1);
	}

	/* À cause d'IPv4 versus IPv6 il faut souvent envoyer ou recevoir
	 * la taille de la sockaddr */
	socklen_t sl = sizeof(struct sockaddr_in);

	/* 4. Échange avec le serveur */
	/* 4.1 Construction de la requête INCP (couche application) */
	uint32_t n;
	if (scanf("%" SCNu32, &n) < 1) {
		fprintf(stderr,
			"Erreur fatale : un entier positif était attendu.");
		return 1;
	}

	n = htonl(n); /* conversion si besoin en gros-boutiste */

	/* 4.2 Envoi de la requête dans le socket */
	/* Remarque : c'est la première fois que le socket est utilisé,
	 * c'est à ce moment que le noyau choisit un port éphémère
	 * et le lie à ce socket.
	 * Envoyer, à travers sock, les 4 octets commençant à l'adresse &n,
	 * sans flag (0), au couple (adresse IP, port distant) renseigné
	 * dans sa, de taille sl octets */
	int status = sendto(sock, &n, 4, 0, (struct sockaddr *) &sa, sl);
	if (status == -1) {
		perror("sendto");
		return 2;
	}

	/* 4.3 Réception de la réponse du serveur */

	ssize_t nb_bytes = recvfrom(sock, &n, 4, 0, (struct sockaddr *) &sa, &sl);
	if (nb_bytes < 4) {
		/* Le protocole INCP interdit que le serveur réponde par autre
		 * chose qu'un message de 4 octets, il y a une erreur */
		perror("recvfrom");
		return 3;
	}
	/* Si sl a été modifiée après recvfrom (ce qui signifie que la taille
	 * de la sockaddr reçue est différente de celle de sockaddr_in) il y a
	 * sans doute un gros problème, mais on laisse de côté cet aspect
	 * désagréable... */

	/* 4.4 Exploitation du message (couche application) */

	n = ntohl(n); /* conversion si besoin dans le boutisme de la machine */

	printf("%" PRIu32 "\n", n);

	close(sock);
	return 0;
}
