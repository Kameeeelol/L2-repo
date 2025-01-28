#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main( int argc, char *argv[]){
    
    
    if (argc == 1) {
        DIR *d = opendir("./");
        struct dirent *ent;
        while ((ent = readdir(d)) != NULL) {
            if (ent->d_name[0] != '.') {
                printf("%s\n", ent->d_name);
            }    
        }
        closedir(d);
        return EXIT_SUCCESS;
    }
    
    if (strcmp(argv[1], "-a") != 0 && strcmp(argv[1], "-i") != 0){
        for (int i=1; i<argc; i++) {
            DIR *d = opendir(argv[i]);
            if ( d == NULL){
                perror("Répertoire introuvable");
                return EXIT_FAILURE;
            }
            struct dirent *ent;
            printf("%s:\n",argv[i]);
            while ((ent = readdir(d)) != NULL) {
                if (ent->d_name[0] != '.') {
                    printf("%s\n", ent->d_name);
                }
            }
        printf("\n");
        closedir(d);
        }
    }

    if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "-i") == 0){
        for (int i=1; i<argc; i++) {
            if ( argv[i][0] == '-') continue;
            DIR *d = opendir(argv[i]);
            if ( d == NULL){
                perror("Répertoire introuvable");
                return EXIT_FAILURE;
            }
            struct dirent *ent;
            printf("%s:\n",argv[i]);
            while ((ent = readdir(d)) != NULL) {
                if (argv[1][1] == 'a') {
                    printf("%s\n", ent->d_name);
                }
                if ( argv[1][1] == 'i' ){
                    if (ent->d_name[0] != '.'){
                        printf("%lu %s\n", (unsigned long)ent->d_ino, ent->d_name);
                    }
                }
                if ( (argv[1][1] == 'i' && argv[2][1] == 'a') || (argv[1][1] == 'a' && argv[2][1] == 'i')) {
                    printf("%lu %s\n", (unsigned long)ent->d_ino, ent->d_name);
                }
            }
            printf("\n");
            closedir(d);
        }
    }
    return 1;
}