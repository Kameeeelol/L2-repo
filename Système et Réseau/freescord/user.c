#include "user.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

/** accepter une connection TCP depuis la socket d'écoute sl et retourner un
 * pointeur vers un struct user, dynamiquement alloué et convenablement
 * initialisé */
struct user *user_accept(int sl)
{
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
	int client_fd = accept(sl, (struct sockaddr *)&client_addr, &addr_len);
	if (client_fd == -1){
		perror("accept");
		exit(-1);
	}
	struct user *user = malloc(sizeof(struct user));
	if (!user){
		perror("malloc");
		free(user);
		close(client_fd);
		exit(-1);
	}
	user->address = malloc(addr_len);
	if (!(user->address)){
		perror("malloc");
		free(user->address);
		close(client_fd);
		exit(-1);
	}
	memcpy(user->address, &client_addr, addr_len);
	user->addr_len = addr_len;
	user->sock = client_fd;	
	return user;
}

/** libérer toute la mémoire associée à user */
void user_free(struct user *user)
{
	if (user){
		free(user->address);
		close(user->sock);
		free(user);
	}
}
