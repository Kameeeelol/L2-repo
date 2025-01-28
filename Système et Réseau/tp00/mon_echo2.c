#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    argv++;
    while(*argv != NULL){
        while(**argv != '\0'){
            putchar(**argv);
            (*argv)++;
        }
        if ( *(argv+1)!= NULL){
            putchar(' ');
        }
        argv++;
    }
    printf("\n");
    return 0;
}