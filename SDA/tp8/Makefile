exec: hachage.o liste.o
	gcc hachage.o liste.o -o exec
    
hachage.o: hachage.c liste.c item.h
	gcc -c hachage.c

liste.o: liste.c liste.h item.h
	gcc -c liste.c

clean:
	rm -f *.o exec