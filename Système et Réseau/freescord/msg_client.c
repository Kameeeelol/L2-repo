#include <stddef.h>
#include <stdint.h>
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
    size_t length = length_msg(msg);
    char *tab = malloc(length);
    *((int *)tab) = msg->fd;
    size_t msg_length = strlen(msg->msg);
    memcpy(tab+sizeof(int), msg->msg, msg_length+1);
    memcpy(tab+sizeof(int)+msg_length+1, msg->pseudo, strlen(msg->pseudo)+1);
    return tab;
}

struct msg_client *deserialisation(char *tab){
    struct msg_client *msg = malloc(sizeof(struct msg_client));
    msg->fd = *((int *) tab);
    uint32_t offset = strlen(tab+sizeof(int)) + 1;
    msg->msg = malloc(offset);
    memcpy(msg->msg, tab + sizeof(int), offset);
    offset += sizeof(int); 
    msg->pseudo = malloc(strlen(tab + offset)+1);
    memcpy(msg->pseudo, tab + offset, strlen(tab + offset)+1);
    return msg;
}

size_t length_msg(struct msg_client *msg){
    return sizeof(int)+strlen(msg->msg)+strlen(msg->pseudo)+2;
}