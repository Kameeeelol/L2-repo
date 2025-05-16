#ifndef MSG_CLIENT_H
#define MSG_CLIENT_H value
#include <stdlib.h>



struct msg_client{
    int fd;
    char *msg;
};

// creat struct msg_client

// struct msg_client *msg_client_creat(int fd, char *msg);

// Fonction de sérialisation 
char *serialisation(struct msg_client *);

// Fonction de désérialisation 
struct msg_client *deserialisation(char *);

// Fonction pour la longueur de la chaine 
size_t length_msg(struct msg_client *msg);



#endif