#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


int main (int argc, char *argv[]){

    if ( argc != 2 ){
        perror("Vous devez le nom du fichier binaire en argument");
        return EXIT_FAILURE;
    }

    char bin = argv[1];

    int fd = open( bin, 0, O_RDONLY );

    if ( fd == -1 ){
        perror("Erreur lors de l'ouverture du fichier bin");
        return EXIT_FAILURE;
    }
    
    int i;
    
    while ( read( fd, &i, sizeof(int)) > 0)   printf("%d", i);
    
    printf("\n");

    close(fd);
    
    return EXIT_SUCCESS;
}