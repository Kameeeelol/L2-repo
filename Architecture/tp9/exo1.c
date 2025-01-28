#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t t = fork();
    if (t < 0) return EXIT_FAILURE;
    if(t==0){
        printf("C'est le fils qui parle\n");
        printf("Mon Pid est : %d\n",getpid());
        printf("Le Pid de mon père est: %d\n", getppid());
    }
    printf("C’est le père qui parle\n");
    printf("Le pid de mon fils est: %d\n",t);
    printf("Mon pid est: %d\n", getpid());
    printf("Le pid du grand-père de mon fils, donc mon père, est: %d\n", getppid());
    return 0;
}