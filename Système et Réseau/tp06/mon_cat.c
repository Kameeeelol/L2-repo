#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFSIZE 1024


int main (int argc, char* argv[]){
    if ( argc == 1 ){
        char buffer[BUFFSIZE];
        ssize_t bytes_reads;
        while ( (bytes_reads = read(0,buffer,BUFFSIZE)) > 0){
            write(1,buffer,bytes_reads);
        }
    } else {
        for ( int i=1; i < argc; i++){
            int fd  = open(argv[i],O_RDONLY);
            if ( fd == -1 ){
                perror("Ouverture");
                return 0;
            }
            char buffer[BUFFSIZE];
            ssize_t bytes_reads;
            while ( (bytes_reads = read(fd,buffer,BUFFSIZE)) > 0){
                write(1,buffer,bytes_reads);
            }
            printf("\n");
            close(fd);
        }
    }
    return 1;
}