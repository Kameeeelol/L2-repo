#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


int main(){
    
    pid_t f1,f2;

    f1 = fork();

    if ( f1 == -1 ){
        perror("La création du premier fils a échouée");
        return EXIT_FAILURE;
    }

    if ( f1 == 0 ){
        printf("Fils 1 de %d: PID : %d\n", getppid(), getpid());
    } else {
        wait(NULL);
        printf("Père (PID = %d) : Fils 1 (PID = %d) a terminé\n", getpid(), f1);
        f2 = fork();
        if ( f2 == -1 ){
            perror("La création du deuxième fils a échouée");
            return EXIT_FAILURE;
        }
        if ( f2 == 0 ){
            printf("Fils 2 PID : %d\n", getpid());
        } else {
            wait(NULL);
            printf("Père (PID = %d) : Fils 2 (PID = %d) a terminé\n", getpid(), f2);
        }
    }
    return EXIT_SUCCESS;
}