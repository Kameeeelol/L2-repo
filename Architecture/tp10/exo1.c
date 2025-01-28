#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
    
    pid_t parent_pid = getpid();
    int fd[2];
    pid_t pid1, pid2;
    
    if (pipe(fd)==-1){
        perror("Erreur lors de la création du tube");
        return EXIT_FAILURE;
    }

    pid1 = fork();

    if(pid1<0){
        perror("Erreur lors de la création du fils F1");
        return EXIT_FAILURE;
    }

    if(pid1 == 0){
        close(fd[0]);
        dup2(fd[1],1);
        execlp("ps", "ps", "-aj", NULL);
        perror("Erreur lors de l'exécution de ps");
        exit(EXIT_FAILURE);
    }

    pid2 = fork();

    if(pid2<0){
        perror("Erreur lors de la création du fils F1");
        return EXIT_FAILURE;
    }    

    if(pid2 == 0){
        close(fd[1]);
        dup2(fd[0],0);
        execlp("tr", "tr", "[:upper:][:lower:]", "[:lower:][:upper:]", NULL);
        perror("Erreur lors de l'exécution de tr");
        exit(EXIT_FAILURE);
    }   

    close(fd[0]);
    close(fd[1]);

     int status;

    pid_t finished_pid = wait(&status);
    if (finished_pid == pid1) {
        printf("Processus F1 (PID: %d)\n", pid1);
    }

    finished_pid = wait(&status);
    if (finished_pid == pid2) {
        printf("Processus F2 (PID: %d) \n", pid2);
    }

    printf("pid%d : ps -aj | tr \"[:upper:][:lower:]\" \"[:lower:][:upper:]\"\n", parent_pid);
    printf("pid%d : ps -aj\n", pid1);
    printf("pid%d : tr \"[:upper:][:lower:]\" \"[:lower:][:upper:]\"\n", pid2);

    return 0;
}
