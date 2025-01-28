#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main ( int argc, char *argv[]){
    

    int pf[2], fp[2];

    pid_t pid;

    int n1, n2, somme;

    if ( pipe(pf) == -1 ){
        perror("La création du tube père-fils est échouée");
        return EXIT_FAILURE;
    }

    if ( pipe(fp) == -1 ){
        perror("La création du tube père-fils est échouée");
        return EXIT_FAILURE;
    }

    if ( (pid = fork()) == -1 ){
        perror("Erreur lors de la création du fils");
        return EXIT_FAILURE;
    }

    if ( pid == 0 ){
        close(pf[1]);
        close(fp[0]);
        read(pf[0], &n1, sizeof(int));
        read(pf[0], &n2, sizeof(int));
        somme = n1 + n2;
        write(fp[1], &somme, sizeof(int));
        return EXIT_SUCCESS;      
    } else {
        close(pf[0]);
        close(fp[1]);
        scanf("%d %d", &n1, &n2);
        write(pf[1], &n1, sizeof(int));
        write(pf[1], &n2, sizeof(int));
        read(fp[0], &somme, sizeof(int));
        printf("%d\n", somme);
        return EXIT_SUCCESS;
    }


}