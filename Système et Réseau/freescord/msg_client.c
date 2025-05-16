#include <stdio.h>
#include <string.h>
#include "msg_client.h"


/*struct msg_client *msg_client_creat(int fd, char *msg){
    struct msg_client * = malloc(sizeof(struct msg_client));
    if (!msg){
        free(msg);
        return NULL;
    }
    msg->msg = malloc(sizeof(msg));
    if (!msg->msg){
        free(msg->msg);
        return NULL;
    }
    msg->fd = fd;
    return msg;
}*/

char *serialisation(struct msg_client *msg){
    char *tab = malloc(sizeof(int)+strlen(msg->msg));
    *((int *)tab) = msg->fd;
    memcpy(tab+sizeof(int), msg->msg, strlen(msg->msg)+1);
    return tab;
}

struct msg_client *deserialisation(char *tab){
    struct msg_client *msg = malloc(sizeof(struct msg_client));
    msg->fd = *((int *) tab);
    msg->msg = malloc(strlen(tab+sizeof(int))+1);
    memcpy(msg->msg, tab + sizeof(int), strlen(tab+sizeof(int))+1);
    return msg;
}

size_t length_msg(struct msg_client *msg){
    return sizeof(int)+strlen(msg->msg)+1;
}