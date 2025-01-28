#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#define CHUNK_SIZE 10240

int main(int argc, char *argv[]){

    if ( argc != 2){
        perror("Il nous faut un seul argument qui est le nom du fichier");
        return EXIT_FAILURE;
    }
    const char *pathname = argv[1];
    int fd = open(pathname,O_RDONLY);
    if(fd == -1){
        perror("Erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }

    unsigned char buffer[CHUNK_SIZE];
    ssize_t bytes_read;
    int chunk_number = 1;

    while ( (bytes_read = read(fd, buffer, CHUNK_SIZE)) > 0){
        char chunk_filename[256];
        sprintf ( chunk_filename, "%s.%d", pathname, chunk_number);
        int chunk_fd = open(chunk_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if( chunk_fd == -1){
            perror("Erreur lors de la creation du fichier");
            close(fd);
            return EXIT_FAILURE;
        }
        if(write(chunk_fd,buffer,bytes_read) != bytes_read){
            perror("Erreur lors de l'ecriture");
            close(fd);
            return EXIT_FAILURE;
        }
        close(chunk_fd);
        printf("partie %d ecris: %s\n", chunk_number, chunk_filename);
        chunk_number++;
    }
    if(bytes_read==-1){
        perror("Erreur lors de la lecture du fichier");
        close(fd);
        return EXIT_FAILURE;
    }
    close(fd);
    printf("le fichier est divisé en %d, parties.\n", chunk_number -1);
    return 0;


}