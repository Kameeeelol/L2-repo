#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]){
    
    if (argc < 2){
        printf("un argument qui porte le nom du fichier est nécessaire ");
        exit(0);
    }
    
    int file = open( argv[1], O_RDONLY);
    
    if ( file == -1){
        printf("Erreur lors de l'ouverture du fichier");
        exit(1);
    }
    
    unsigned char buffer[16];
    ssize_t n_bytes;
    n_bytes = read (file, &buffer, 16);

    if (n_bytes == -1){
        printf("Votre fichier doit contenir au moins 16 octets");
        close(file);
        exit(1);
    }

    printf("Les 16 premiers octets en hexadécimal :\n");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    off_t fileSize = lseek(file, 0, SEEK_END);
    if (fileSize < 16) {
        printf("Le fichier doit avoir au moins 16 octets");
        close(file);
        exit(1);
    }

    lseek(file, -16, SEEK_END);
    n_bytes = read(file, buffer, 16);

    printf("Les 16 derniers octets en hexadécimal :\n");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", buffer[i]);
    }
    printf("\n");
    
    close(file);
    return 0;


}