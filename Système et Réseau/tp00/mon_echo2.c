#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int agrc, char **argv){
    argv++;
    while (NULL != *argv) {
        while (**argv != '\0') {
            putchar(**argv);
            (*argv)++;
        }
        if (*(argv+1) != NULL){
            putchar(' ');
        }
        argv++;
    }    
    putchar('\n');
    return 0;
    
}