#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int add1odd(char *fname){
    int fd = open(fname,O_RDWR);
    if ( fd == -1) {
        perror("Erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }

    int value;
    ssize_t bytesRead;

    while (bytesRead = read(fd,&value,sizeof(int))==sizeof(int)){
        if ( value % 2 == 0) value += 1;
        if (lseek(fd, -sizeof(int), SEEK_CUR) == -1) {
                perror("Erreur avec lseek");
                close(fd);
                return EXIT_FAILURE;
        }
        if (write(fd, &value, sizeof(int)) == -1) {
                perror("Erreur d'écriture dans le fichier");
                close(fd);
                return EXIT_FAILURE;
        }
    }
    if (bytesRead == -1) {
        perror("Erreur de lecture du fichier");
        close(fd);
        return EXIT_FAILURE;
    }
    close(fd);
    return 0;

    
}

