#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


int main(){
    pid_t fd = fork();
    if(fd<0){
        perror("fork failed");
        return EXIT_FAILURE;
    }
    if ( fd == 0 ){
        printf("C'est le fils qui parle, Mon pid est : %d, et le pid de mon père est : %d\n", getpid(), getppid());
    }else{
    printf("C'est le père qui parle, le pid de mon fils est %d, et mon pid est %d, et le pid de mon père est %d\n",fd,getpid(),getppid());
    }
    return 0;

}