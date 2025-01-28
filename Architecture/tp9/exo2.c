#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    char path[200];
    while(1){
        printf("Entrez le chemin absolu ou relatif: \n");
        scanf("%s", path);
        if (strcmp(path, "exit") == 0) {
            printf("Programme terminé.\n");
            break;
        }
        pid_t t = fork();
        if(t<0) return EXIT_FAILURE;
        if(t==0){
            execl("/bin/ls", "ls", "-l", path, NULL);
            perror("Erreur lors de l'exécution de 'ls'\n");
            exit(1);
        }else{
            wait();
        }
    }
    return 1;
}