// Kamel Seggari 12203821
// Aris Ait-Mohammed 
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
    
    if ( argc != 4 ){
        printf("Nombres d'arguments incorrect\n");
        return EXIT_FAILURE;
    }
    
    char *fin = argv[1];
    char *mot = argv[2];
    char *fout = argv[3];

    int fd[2];
    if ( pipe(fd) == -1 ){    // si le tube n'est pas créer ça renvoi une erreure
        perror("Erreur lors de la création du tube");
        return EXIT_FAILURE;
    }

    pid_t fils1 = fork();  // Création du premier fils
    if ( fils1 == -1 ){     // Renvoi un erreur si le fils n'est pas créer
        perror("Erreur lors de la création du fils numero 1\n");
        return EXIT_FAILURE;
    }

    if ( fils1 == 0 ){  // Rentre dans le if si c'est le fils1 qui execute
        close(fd[0]);
        int fin_fd = open(fin, O_RDONLY); // Ouvre le fichier en mode lecture seulement 
        if ( fin_fd == -1){ // Renvoi une erreur si le fichier n'est pas ouvert 
            perror("Erreur lors de l'ouverture du fichier donné comme argument\n");
            return EXIT_FAILURE;
        }
        dup2(fin_fd, 0); // Redérige l'entrée standard vers le fichier fin 
        dup2(fd[1], 1); // Redérige la sortie standard vers le tube 
        close(fd[1]);
        execlp("cat", "cat", (char *)NULL); // Executer cat 
        perror("Erreur lors de l'exécution de cat");
        return EXIT_FAILURE;
    }

    pid_t fils2 = fork(); // Création du deuxième fils 
    if ( fils2 == -1 ){ // Si il n'est pas créer ça renvoi une erreur 
        perror("Erreur lors de la création du fils numero 1\n");
        return EXIT_FAILURE;
    }

    if ( fils2 == 0 ){ // Rentre dans le if si c'est le fils qui est execute 
        close(fd[1]); // Ferme le point d'écriture du tube 
        int fout_fd = open(fout, O_WRONLY | O_CREAT | O_TRUNC, 0644); // Ouvre le fichier en mode écriture seulement et le crée si il n'éxiste pas
        if (fout_fd == -1) { // Renvoi une erreur si le fichier n'est pas ouvert
            perror("Erreur lors de l'ouverture du fichier de sortie\n");
            return EXIT_FAILURE;
        }
        dup2(fd[0], 0); // Redérige l'entrée standard vers le tube 
        close(fd[0]);
        dup2(fout_fd, 1); // Redérige la sortie standard vers le fichier donné en argument 
        close(fout_fd);
        execlp("grep", "grep", mot, (char *)NULL); // Executer grep mot
        perror("Erreur lors de l'exécution de grep\n");
        return EXIT_FAILURE;
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(fils1,NULL,0);
    waitpid(fils2,NULL,0);
    printf("Les deux fils sont terminés. Le fichier '%s' contient les résultats.\n", fout);
    return EXIT_SUCCESS;
}