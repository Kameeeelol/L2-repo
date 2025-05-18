#ifndef USER_H
#define USER_H
#include <sys/socket.h>
#include <netinet/in.h>

#define pseudo_len 17

struct user {
	struct sockaddr *address;
	socklen_t addr_len;
	int sock;
	char pseudo[pseudo_len];
	/* autres champs éventuels */
};

struct user *user_accept(int sl);
void user_free(struct user *user);

#endif /* ifndef USER_H */
