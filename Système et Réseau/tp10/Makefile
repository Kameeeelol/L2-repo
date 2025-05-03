CC=gcc
CFLAGS=-Wall -g -Wpointer-arith
LDFLAGS=-pthread -Wall

all: client_quizz serveur_quizz

client_quizz: client_quizz.o sys_utils.o question.o
serveur_quizz: serveur_quizz.o sys_utils.o question.o

client_quizz.o: client_quizz.c sys_utils.h
question.o: question.c question.h sys_utils.h
serveur_quizz.o: serveur_quizz.c sys_utils.h question.h
sys_utils.o: sys_utils.c sys_utils.h
clean:
	rm -f client_quizz.o serveur_quizz.o sys_utils.o question.o \
		client_quizz serveur_quizz
