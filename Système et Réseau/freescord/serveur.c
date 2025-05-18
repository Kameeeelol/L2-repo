/* Kamal Seggari 12203821
Je déclare qu'il s'agit de mon propre travail.
Ce travail a été réalisé intégralement par un être humain. */


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
#include "msg_client.h"

#define WELCOME_MSG                                                  \
    "Bienvenue !\n"                                     			\
    "Ici, vous pouvez chatter en direct.\n"                           \
    "Pour commencer, choisissez un pseudonyme.\n"                     \
    "\n"  // ligne vide pour terminer

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

/*fonction qui gère la partie pour choisir le pseudonyme
du client*/
void *pseudo_client(void *arg);


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
		// thread pour gérer le pseudonyme puis s'en charge de lancer le thread handle_client()
		pthread_t th_nick;
        if (pthread_create(&th_nick, NULL, pseudo_client, user) != 0) {
            perror("pthread");
            user_free(user);
            continue;
        }
		pthread_detach(th_nick);
	}
	close(listen_sock);
	return 0;
}

void *handle_client(void *user)
{
	struct user *client = (struct user *)user;
	struct msg_client msg;
	msg.fd = client->sock;
	msg.pseudo = client->pseudo;
	printf("[%d] Pseudonyme : %s\n", msg.fd, msg.pseudo);
	char buff[BUFFER_SIZE];
	ssize_t n;
	char *msg_to_send;
	while ( (n = read(client->sock, (char *) buff, sizeof(buff))) > 0){
		buff[n] = '\0';
		msg.msg = buff;
		msg_to_send = serialisation(&msg); // sérialisation du message pour l'envoyer au répéteur (merci Mr Schulz)
		//memset(msg_to_send, 0, sizeof(msg_to_send));
		//sprintf(msg_to_send, "[%d] says: %s", client->sock, buff);
		pthread_mutex_lock(&mutex_list);
		if ((write(tube[1], msg_to_send, length_msg(&msg))) == -1){
			perror("write");
			break;
		}
		pthread_mutex_unlock(&mutex_list);
		printf("Got message from [%d] %s : %s", msg.fd ,client->pseudo, buff); // le serveur reçoit les messages des clients (il a le droit de savoir ce qu'il se passe aussi)
	}
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
		perror("socket");
		exit(-1);
	}
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
		perror ("bind");
		close(sock);
		exit(-1);
	}
	if (listen(sock, 10) == -1){
		perror ("listen");
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
		perror ("accepte");
		exit(-1);
	}
	return client_fd;
}

void *repeteur(void *arg){
	char buff[BUFFER_SIZE];
	ssize_t n;
	for (;;){
		n = read(tube[0], (char *)buff, sizeof(buff));
		struct msg_client *msg = deserialisation((char *)buff); // désérialisation 
		size_t prefix_len = snprintf(NULL, 0, "[%d] %s : ", msg->fd, msg->pseudo); // la taille du préfix (on veut un truc beau quand meme) 
		size_t msg_len = strlen(msg->msg);
		size_t total_len = prefix_len + msg_len + 1;
		char *msg_to_send = malloc(total_len);
		if (!msg_to_send) {
            perror("malloc");
            free(msg->msg);
            free(msg);
            continue;
        }
		snprintf(msg_to_send, total_len, "[%d] %s : %s", msg->fd, msg->pseudo ,msg->msg); // initialisation du message a envoyer
		if (n < 0) continue;
		pthread_mutex_lock(&mutex_list);
        struct node *curr = list_utilisateurs->first;
		while (curr != NULL) {
			struct user *client = (struct user *)curr->elt;
			if (client->sock != msg->fd){  // envoi du message a tout le monde sauf a l'éxpiditeur reconnu grace a son fd (merci la sérialisation)
				write(client->sock, msg_to_send, strlen(msg_to_send));
				}
			curr = curr->next;
        }
        pthread_mutex_unlock(&mutex_list);
		free(msg->msg);
		free(msg_to_send);
		free(msg);
	}
	return NULL;
}

void *pseudo_client(void *arg) {
    struct user *client = arg;
	write(client->sock, WELCOME_MSG, strlen(WELCOME_MSG));
    char line[BUFFER_SIZE];
    int status = -1;
	/*const char *pseudos_interdits[] = {"fdp", "Hitler", "Nazi", "connard", "zizi",
										""};*/
    while (status != 0){
        ssize_t n = read(client->sock, line, sizeof(line)-1);
        if (n <= 0) { client->pseudo[0] = '\0'; return NULL; }
        line[n] = '\0';

        // Vérif de nickname 
        if (strncmp(line, "nickname ", 9) != 0) {
            status = 3;
        } else {
            char *nick = line + 9;
            nick[strcspn(nick, "\r\n")] = '\0';
			// vérif du pseudo 
            if (strlen(nick) == 0 || strlen(nick) >= pseudo_len || strchr(nick, ':') || strcmp(nick, "fdp") == 0 || strcmp(nick, "Hitler") == 0 ||
			strcmp(nick, "Nazi") == 0) {
                status = 2;
            } else {
                // vérif de l'unicité 
				int exists = 0;
				pthread_mutex_lock(&mutex_list);
                struct node *curr = list_utilisateurs->first;
				while (curr) {
    				struct user *u = (struct user *)curr->elt;
    				if (strcmp(u->pseudo, nick) == 0) {
        			exists = 1;
        			break;
    				}
    			curr = curr->next;
				}
				pthread_mutex_unlock(&mutex_list);
                status = exists ? 1 : 0; // tout va bien ? 
                if (status == 0) {
                    strncpy(client->pseudo, nick, pseudo_len);
					client->pseudo[pseudo_len-1] = '\0';
                }
            }
        }
		char buff[BUFFER_SIZE];
			const char *msg_to_client;
			// envoie un message au client selon le status d'érreur
			switch (status) {
				case 0:
				msg_to_client = "Parfait"; break;
				case 1:
				msg_to_client = "Pseudonyme déjà pris"; break;
				case 2:
				msg_to_client = "Pseudonyme interdit ou trop court"; break;
				case 3:
				msg_to_client = "Commande mal formulée, faites nickname <votre-pseudo>"; break;
				default:
				msg_to_client = "ERREUR 404"; break;
			}
			int len = snprintf(buff, sizeof(buff), "%d %s\n", status, msg_to_client);
        	write(client->sock, buff, len);
    }
	pthread_mutex_lock(&mutex_list);
	list_add(list_utilisateurs, client); // ajout du client a la list si tout est nickel 
	pthread_mutex_unlock(&mutex_list);
	pthread_t th;
	if (pthread_create(&th, NULL, handle_client, client) != 0){ // lancement du thread handle_client qui gère chaque client
		perror("pthread");
		user_free(client);
	}else{
		pthread_detach(th);
	}
    return NULL;
}
