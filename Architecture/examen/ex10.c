#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    
    int f1f2[2], f2f3[2];
    pid_t f1, f2, f3;

    if ( pipe(f1f2) == -1){
        perror("La création du tube est échouée");
        return EXIT_FAILURE;
    }

    if ( pipe(f2f3) == -1){
        perror("La création du tube est échouée");
        return EXIT_FAILURE;
    }

    if ( (f1 = fork()) == -1){
        perror("La création du 1er fils a échouée");
        return EXIT_FAILURE;
    }

    if ( f1 == 0){
        close(f1f2[0]);
        dup2(f1f2[1], 1);
        execl("/bin/ps","ps","ax", NULL);
        perror("Erreur lors de l'execution de la commande");
        return EXIT_FAILURE;
    }

    if ( (f2 = fork()) == -1){
        perror("La création du 1er fils a échouée");
        return EXIT_FAILURE;
    }

    if ( f2 == 0 ){
        close(f1f2[1]);
        dup2(f1f2[0], 0);
        close(f2f3[0]);
        dup2(f2f3[1], 1);
        execl("/usr/bin/grep", "grep", "bash", NULL);
        perror("Erreur lors de l'execution de la commande");
        return EXIT_FAILURE;
    }
    
    close(f1f2[0]);
    close(f1f2[1]);

    if ( (f3 = fork()) == -1){
        perror("La création du 1er fils a échouée");
        return EXIT_FAILURE;
    }
    
    if ( f3 == 0 ){
        close(f2f3[1]);
        dup2(f2f3[0], 0);
        execl("/usr/bin/wc", "wc", "-l", NULL);
        perror("Erreur lors de l'execution de la commande");
        return EXIT_FAILURE;
    }

    close(f2f3[0]);
    close(f2f3[1]);

    wait(NULL);
    wait(NULL);
    wait(NULL);
    
    return EXIT_SUCCESS;
    
}