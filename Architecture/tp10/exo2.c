#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_LENGTH 32

typedef struct {
    int length;
    int array[MAX_LENGTH];
}Data;


int main(){
    
    pid_t pid1; 
    int pf[2],fp[2];

    if(pipe(pf)== -1 || pipre(fp)== -1){
        perror("erreur lors de la création des tubes\n");
        return EXIT_FAILURE;
    }
    
    Data tab;
    int length;
    printf("Entrez la longueur de la suite d'entiers ≤ 32 : ");
    scanf("%d\n", &length);
    tab.length = length;

    if(tab.length > 32 || tab.length <= 0){
        perror("la langueur doit etre ≤ 32");
        return EXIT_FAILURE;
    }

    printf("Entrez %d entiers",tab.length);
    for(int i = 0; i < tab.length; i++){
        scanf("%d", tab.array[i]);
    }

    if ((pid1=frok())!=0){
        int tab[32];
        int i = 0;
        printf("Veuillez rentrer une suite d'entiers ≤ 32 :\n");
        scanf("%d",&tab[i]);
        while(tab[i]){
            close(pf[0]);
            close(fp[1]);
            write(,tab[i],sizeof(tab[i]));
            scanf()

        }
    }
}