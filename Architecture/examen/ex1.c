#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    const char *pathname = argv[1];
    if ( argc != 2 ){
        perror("Il faut uniquement un seul argument qui est le nom du fichier");
        return EXIT_FAILURE;
    }

    int fd = open(pathname, O_RDONLY );
    if ( fd == -1){
        perror("l'ouverture du fichier est échouée");
        return EXIT_FAILURE;
    }

    unsigned char buffer[16];
    ssize_t bytes_read;

    bytes_read = read(fd,buffer,16);
    if(bytes_read == -1){
        perror("La lecture du fichier a échouée");
        return EXIT_FAILURE;
    }

    printf("Les 16 premiers octets sont :\n");
    for ( int i = 0; i<16; i++){
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    off_t file_size = lseek(fd, 0, SEEK_END);
    if( file_size == -1){
        perror("Erreur lors de la recherche");
        return EXIT_FAILURE;
    }
    if( file_size < 16){
        perror("Le fichier a moins de 16 octets");
        return EXIT_FAILURE;
    }

    if(lseek(fd,-16,SEEK_END) == -1){
        perror("Erreur lors de la recherche des 16 derniers bytes");
        return EXIT_FAILURE;
    }
    bytes_read = read(fd,buffer,16);
    if( bytes_read == -1){
        perror("erreur de la lecture du fichier");
        return EXIT_FAILURE;
    }
    if(bytes_read < 16){
        perror("erreur lors de la lecture des 1§ derniers bytes");
        return EXIT_FAILURE;
    }
    printf("Les 16 derniers bytes sont:\n");
    for(int i = 0; i<16; i++){
        printf("%02x ", buffer[i]);
    }
    printf("\n");
    close(fd);
    return 0;

}