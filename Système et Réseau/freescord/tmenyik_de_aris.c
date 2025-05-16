#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include "buffer/buffer.h"
#include "utils.h"
#include <string.h>

#define PORT_FREESCORD 4321
#define BUFSIZE 1024

/** se connecter au serveur TCP d'adresse donnée en argument sous forme de
 * chaîne de caractère et au port donné en argument
 * retourne le descripteur de fichier de la socket obtenue ou -1 en cas
 * d'erreur. */
int connect_serveur_tcp(char *adresse, uint16_t port);

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <IP serveur>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int sockfd = connect_serveur_tcp(argv[1], PORT_FREESCORD);
	if (sockfd < 0) {
		fprintf(stderr, "Erreur de connexion au serveur\n");
		exit(EXIT_FAILURE);
	}
  char buff[BUFSIZ];

  struct pollfd fds[2] = {
  {fds[0].fd = STDIN_FILENO,
  fds[0].events = POLLIN},        // surveiller les entrées

  {fds[1].fd = sockfd,
  fds[1].events = POLLIN}
  };       // surveiller si la socket a des données à lire
  /*struct pollfd fds[2] = {
    { .fd = STDIN_FILENO, .events = POLLIN },
    { .fd = sockfd,       .events = POLLIN }
  };
  struct pollfd fds[2];

  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  fds[1].fd = sockfd;
  fds[1].events = POLLIN;
*/


  buffer *bufsock = buff_create(sockfd, BUFSIZE);
  char ligne[BUFSIZE];


	while (1) {
		//printf("\n ecrivez qq chose:\n ");
		fflush(stdout);

    while (buff_ready(bufsock)) {
        // Ici, un compromis : essayer buff_fgets, mais vérifier si la ligne est complète
        if (buff_fgets(bufsock, ligne, sizeof(ligne))) {
            if (strcmp(ligne, "\n") == 0 || strcmp(ligne, "") == 0) {
        // ligne vide détectée, fin du message welcome
              break;
            }

            size_t len = strlen(ligne);
                write(STDOUT_FILENO, ligne, len);

        } else {
            // Pas de ligne complète  on attend
            break;
        }
    }

    int ret = poll(fds, 2, -1);  // -1  blocage indéfini

    if (ret < 0) {
        perror("poll");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (fds[0].revents & POLLIN) {
		// Lire une ligne depuis le terminal
      if (!fgets(buff, sizeof(buff), stdin)) {
        break; // EOF (Ctrl+D) ou erreur
      }

      //envoie au serveur
      ssize_t len = strlen(buff);
      ssize_t sent = 0;
      while (sent < len) {
        ssize_t n = write(sockfd, buff + sent, len - sent);
        if (n <= 0) {
          perror("write");
          break;
        }
        sent += n;
      }

    }

    if (fds[1].revents & POLLIN) {
      if (!buff_ready(bufsock)) {
        // Forcer la lecture du socket pour remplir le tampon
          int c = buff_getc(bufsock);
          if (c == -1) {
            perror("read");
            break;
          }
        // Reposer le caractère pour qu’il soit relu par buff_fgets
          if (buff_ungetc(bufsock, c) == -1) {
              fprintf(stderr, "Erreur : double ungetc ou buffer corrompu\n");
              break;
          }
      }
    }

}




  buff_free(bufsock);
	close(sockfd);
	return 0;
}

int connect_serveur_tcp(char *adresse, uint16_t port)
{
  int sockfd;
  struct sockaddr_in serv_addr;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, adresse, &serv_addr.sin_addr) <= 0) {
		perror("inet_pton");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	// Tentative de connexion
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
		perror("connect");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	return sockfd;
}