#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list/list.h"
#include "user.h"

#define PORT_FREESCORD 4321
#define BUFFER_SIZE 1024  // Déclaration globale de la taille du buffer
int tube[2]; // Déclaration globale du tube 
struct list *list_utilisateurs; // Déclaration globale d'une struct list pour les clients
pthread_mutex_t mutex_list; // Déclaration global du mutex pour gérer les insértions et les removes

/** Gérer toutes les communications avec le client renseigné dans
 * user, qui doit être l'adresse d'une struct user */
void *handle_client(void *user);
/** Créer et configurer une socket d'écoute sur le port donné en argument
 * retourne le descripteur de cette socket, ou -1 en cas d'erreur */
int create_listening_sock(uint16_t port);
/** Accepte les demandes de connection entrant sur la socket d'écoute sock.
 * Renvoi le discripteur de fichier de la socket d'échange créer ou -1
 * en cas d'érreurs */
 int connection_au_client(int sock);
 /** Pour chaque fois qu'il écoute dans le tube, les réécrit dans toutes
  * sockets de tous les utilisateurs.
  */
 void *repeteur(void *arg);

int main(int argc, char *argv[])
{
	int listen_sock = create_listening_sock(PORT_FREESCORD);
	printf("[Server] Waiting...\n");
	list_utilisateurs = list_create();
	 
	if (pipe(tube) == -1){
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pthread_t th_repeteur;
	if ( pthread_create(&th_repeteur, NULL, &repeteur, NULL) != 0){
		perror("thread");
		free(list_utilisateurs);
		exit(EXIT_FAILURE);
	}

	pthread_mutex_init(&mutex_list,NULL);

	for (;;){
		struct user *user = user_accept(listen_sock);
		printf("Client connected\n");
		printf("[Server] Waiting...\n");
		pthread_mutex_lock(&mutex_list);
		list_add(list_utilisateurs, user);
		pthread_mutex_unlock(&mutex_list);
		pthread_t th;
		if (pthread_create(&th, NULL, handle_client, user) != 0){
			perror("pthread");
			user_free(user);
		}else{
			pthread_detach(th);
		}
	}
	close(listen_sock);
	return 0;
}

void *handle_client(void *user)
{
	struct user *client = (struct user *)user;
	char buff[BUFFER_SIZE];
	ssize_t n;
	char msg_to_send[BUFFER_SIZE];
	while ( (n = read(client->sock, (char *) buff, sizeof(buff))) > 0){
		buff[n] = '\0';
		memset(msg_to_send, 0, sizeof(msg_to_send));
		sprintf(msg_to_send, "[%d] says: %s", client->sock, buff);
		pthread_mutex_lock(&mutex_list);
		if ((write(tube[1], msg_to_send, strlen(msg_to_send))) == -1){
			perror("write");
			break;
		}
		pthread_mutex_unlock(&mutex_list);
		printf("[%zd] Got message : %s", n, buff);
	}
	if (n < 1) perror("read:");
	pthread_mutex_lock(&mutex_list);
    list_remove_element(list_utilisateurs, client);
    pthread_mutex_unlock(&mutex_list);
	user_free(client);
	printf("Client disconnected\n");
	return NULL;
}

int create_listening_sock(uint16_t port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1){
		perror("socket:");
		exit(-1);
	}
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
		perror ("bind:");
		close(sock);
		exit(-1);
	}
	if (listen(sock, 10) == -1){
		perror ("listen:");
		close(sock);
		exit(-1);
	}
	printf("[Server] Created server socket fd\n");
	printf("[server] Bound socket port %d\n", PORT_FREESCORD);
	printf("[Server] listening on port %d\n", PORT_FREESCORD); 
	return sock;
}

int connection_au_client(int sock)
{
	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));
	socklen_t client_len = sizeof(client);
	int client_fd = accept(sock, (struct sockaddr *)&client, &client_len);
	if (client_fd == -1){
		perror ("accepte:");
		exit(-1);
	}
	return client_fd;
}

void *repeteur(void *arg){
	char buff[BUFFER_SIZE];
	ssize_t n;
	for (;;){
		n = read(tube[0], (char *)buff, sizeof(buff));
		if (n < 0) continue;
		pthread_mutex_lock(&mutex_list);
        struct node *curr = list_utilisateurs->first;
		while (curr != NULL) {
			struct user *client = (struct user *)curr->elt;
			if (write(client->sock, buff, n) != n) {
                perror("write");
            }
			curr = curr->next;
        }
        pthread_mutex_unlock(&mutex_list);
	}
	return NULL;
}