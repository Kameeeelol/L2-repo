#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 1024

int main (int argc, char* argv[]){

    if ( argc != 3 ){
        perror("cp:");
        return 1;
    }

    int fd = open(argv[1],O_RDONLY);

    if ( fd == -1 ){
        perror("open:");
        return 1;
    }

    char buffer[BUFSIZE];
    ssize_t bytes_reads;

    int fd2 = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if ( fd2 == -1 ){
        perror("open:");
        return 1;
    }
    
    while ( (bytes_reads = read(fd,buffer,BUFSIZE)) > 0 ){
        write(fd2,buffer,bytes_reads);
    }

    close(fd);
    close(fd2);
    return 0;


}