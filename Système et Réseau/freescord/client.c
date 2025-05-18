/* Kamal Seggari 12203821
Je déclare qu'il s'agit de mon propre travail.
Ce travail a été réalisé intégralement par un être humain. */

#include <stddef.h>
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
#define BUFFER_SIZE 1024  // Déclaration globale de la taille du buffer

/** se connecter au serveur TCP d'adresse donnée en argument sous forme de
 * chaîne de caractère et au port donné en argument
 * retourne le descripteur de fichier de la socket obtenue ou -1 en cas
 * d'erreur. */
int connect_serveur_tcp(char *adresse, uint16_t port);

int main(int argc, char *argv[])
{
	if (argc < 2){
		perror("argc");
		exit(EXIT_FAILURE);
	}
	char *adresse = argv[1];
	int connect_sock = connect_serveur_tcp(adresse, PORT_FREESCORD);
	if (connect_sock < 0){
		printf("Connexion failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Connected to server\n");
	
	struct pollfd fds[2];
   
    fds[0].fd = 0;   
    fds[0].events = POLLIN;
    fds[1].fd = connect_sock;    
    fds[1].events = POLLIN;
	
	buffer *buffsock = buff_create(connect_sock, BUFFER_SIZE);
	char ligne[BUFFER_SIZE];
	char buff[BUFFER_SIZE];
	
	for (;;){
        
		fflush(stdout);
		while (buff_ready(buffsock)) {
			if (buff_fgets(buffsock, ligne, sizeof(ligne))) {
				if (strcmp(ligne, "\n") == 0 || strcmp(ligne, "\r\n") == 0) break;
				size_t length = strlen(ligne);
				write(1, ligne, length);
			}else {
				break; // ligne incomplete donc on attend 
			}
		}
		
		int attendre = poll(fds, 2, -1);
        if (attendre == -1) {
            perror("poll");
            break;
        }
		if (fds[0].revents){
			if ( fgets(buff, sizeof(buff), stdin) != NULL){
				size_t len = strlen(buff);
				if (write(connect_sock, (char *)buff, len) != len){
					perror("write:");
					break;
				}
			}
		}
		if (fds[1].revents) {
			if (!buff_ready(buffsock)) {
				int c = buff_getc(buffsock);
				if (c == -1){
					perror("read");
					break;
				}
				// Reposer le caractère pour qu’il soit relu par buff_fgets
          	if (buff_ungetc(buffsock, c) == -1) {
              	fprintf(stderr, "Erreur : double ungetc ou buffer corrompu\n");
              	break;
          }

			}
		}
		/*if(fds[1].revents){	
			ssize_t n = read(connect_sock, (char *)buff, sizeof(buff)-1);
			if (n < 0){
				perror("read:");
				break;
			}
			if (n == 0){
				fprintf(stderr, "Server closed\n");
				break;
			}
			buff[n] = '\0';
			fputs(buff, stdout);	
		}*/
	}
	buff_free(buffsock);
	close(connect_sock);
	return 0;
}

int connect_serveur_tcp(char *adresse, uint16_t port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1){
		perror ("sock:");
		exit(-1);
	}
	struct sockaddr_in server;
	memset (&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if ((inet_pton(AF_INET, adresse, &server.sin_addr)) <= 0){
		perror("inet_pton:");
		exit(-1);
	}
	if ((connect(sock, (struct sockaddr *)&server, sizeof(server))) == -1){
		perror("connect:");
		exit(-1);
	}
	/* pour éviter les warnings de variable non utilisée */
	return sock;
}
