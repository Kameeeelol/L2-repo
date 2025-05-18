CC = gcc
CFLAGS = -g -Wall -Wvla -std=c99 -pthread -D_XOPEN_SOURCE=700
LDFLAGS = -pthread -Wall
EXE = srv clt
TEST = list/test_list buffer/test_buffer

all: $(EXE)

# Compilation du serveur
srv: serveur.o list/list.o user.o msg_client.o utils.o buffer/buffer.o
	$(CC) $(LDFLAGS) $^ -o $@

# Compilation du client
clt: client.o buffer/buffer.o utils.o
	$(CC) $(LDFLAGS) $^ -o $@

# Compilation des .o (fichiers objets)
client.o: client.c user.h buffer/buffer.h utils.h
serveur.o: serveur.c list/list.h user.h msg_client.h utils.h buffer/buffer.h
user.o: user.c user.h
msg_client.o: msg_client.c msg_client.h
utils.o: utils.c utils.h

list/list.o: list/list.c list/list.h
list/test_list.o: list/test_list.c list/list.h
list/test_list: list/test_list.o list/list.o
	$(CC) $(LDFLAGS) $^ -o $@

buffer/buffer.o: buffer/buffer.c buffer/buffer.h
buffer/test_buffer.o: buffer/test_buffer.c buffer/buffer.h
buffer/test_buffer: buffer/test_buffer.o buffer/buffer.o
	$(CC) $(LDFLAGS) $^ -o $@

# Commande pour exécuter les tests avec valgrind
test: testlist testbuffer

testlist: list/test_list
	valgrind --leak-check=full ./$<

testbuffer: buffer/test_buffer
	valgrind --leak-check=full ./$<

clean:
	rm -f *.o list/*.o buffer/*.o $(EXE) $(TEST)
