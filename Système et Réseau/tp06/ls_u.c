#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>


int main (int argc, char* argv[]){

    int tube[2];
    pipe(tube);
    
    if (fork() == 0){
        close(tube[0]);
        dup2(tube[1],1);
        execlp("ls","ls","/",NULL);
    } else if ( fork()==0 ){
        close(tube[1]);
        dup2(tube[0],0);
        execlp("grep","grep","u",NULL);
    }
    close(tube[1]);
    close(tube[0]);
    wait(NULL);
    wait(NULL);
    return 0;
}