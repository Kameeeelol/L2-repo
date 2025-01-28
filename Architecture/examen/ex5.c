#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    char path;
    while(1){
        printf("Entrez le chemin absolu ou relatif ou 0 pour quitter : \n");
        scanf("%c",&path);
        if (path == '0') break;
        pid_t pid = fork();
        if (pid == -1){
            perror("Erreur lors que la creation du fils");
            return EXIT_FAILURE;
        }else if(pid == 0){
            execl("/bin/ls", "ls", "-l", path, NULL);
            perror("Erreur lors de l'exel");
            return EXIT_FAILURE;
        }


    }
    printf("fin du programme.\n");
    return 0;
}