#include <stdlib.h>
#include <stdio.h>

extern char **environ;

int main(){
    char **env = environ;
    while (NULL != *env) {
        printf("%s\n", *env);
        env++;
    }
    return 0;
}