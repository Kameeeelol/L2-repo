CC=gcc
CFLAGS=-Wall -Wfatal-errors -Wpointer-arith -g
LDFLAGS=-Wall -g -pthread
OBJS=client_udp_incp.o serveur_udp_incp.o

all: client serveur

client: client_udp_incp.o
	$(CC) $(LDFLAGS) client_udp_incp.o -o $@
serveur: serveur_udp_incp.o
	$(CC) $(LDFLAGS) serveur_udp_incp.o -o $@
client_udp_incp.o: client_udp_incp.c
serveur_udp_incp.o: serveur_udp_incp.c

.PHONY: clean
clean:
	rm -f client serveur serveur_threads $(OBJS)
