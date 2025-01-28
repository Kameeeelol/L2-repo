#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct rec_t {
    char name[20];
    char surname[20];
    int ine;
} rec_t;

int rec_count(int fd){
    off_t size = lseek(fd,0,SEEK_END);
    if(size == -1){
        printf("erreur");
        EXIT_FAILURE;
    }
    return size / sizeof(rec_t);
}

int rec_read(int fd, int n, rec_t *pr){
    if (n<0) return 0;
    off_t offset = n * sizeof(rec_t);
    if(lseek(fd,offset,SEEK_SET)==-1) return 0;
    ssize_t reead = read(fd,pr,sizeof(rec_t));
    if(reead == -1) return EXIT_FAILURE;
    return 1;
}

int rec_search(int fd, unsigned ine){
    rec_t r;
    int pos = 0;
    if(lseek(fd,0,SEEK_SET)==-1)return -1;
    while(read(fd,&r,sizeof(rec_t))==sizeof(rec_t)){
        if (r.ine == ine){
            return pos;
        }
        pos++;
    }
    return -1;
}

int rec_write(int fd, int n, rec_t *pr){
    if (n<0) return 0;
    int rec = rec_count(fd);
    if(n>rec) return 0;
    off_t offset = n * sizeof(rec_t);
    if(lseek(fd,offset,SEEK_SET)==-1) return 0;
    ssize_t wrrite = write(fd,pr,sizeof(rec_t));
    if (wrrite != sizeof(rec_t)) return 0;
    return 1;

    }

int main(int argc, char *argv[]) {
  int fd, go = 1;
  char fname[256];
  unsigned a;
  
  do {
    printf("Fichier des enregistrements [256 chars max] : ");
    scanf("%s", fname);

    if ((fd = open(fname, O_RDWR | O_CREAT, 0660)) == -1)
      printf("Impossible d'ouvrir le fichier %s\n", fname);
  } while (fd == -1);

  while(go) {
    printf(">>> Choisir une action\n");
    printf("  [ 0: exit | 1: read | 2: write | 3: count | 4: search ] : ");
    scanf("%u", &a);
    switch(a) {
    case 0: {
      go = 0;
      printf("Au revoir !\n\n");
      break;
    }
    case 1: {
      int n, nrec = rec_count(fd);
      rec_t rec;
      if (nrec == 0) {
	printf("Le fichier est vide.\n\n");
	break;
      }
      printf("Position de l'enregistrement à lire [0..%d] : ", nrec-1);
      scanf("%d", &n);
      if (n < 0 || n >= nrec) {
	printf("Position de l'enregistrement non valid.\n\n");
	break;
      }
      rec_read(fd, n, &rec);
      printf("%s %s %08d\n\n", rec.name, rec.surname, rec.ine);
      break;
    }
    case 2: {
      int n, nrec = rec_count(fd);
      rec_t rec;
      printf("Position de l'enregistrement à écrire [0..%d] : ", nrec);
      scanf("%d", &n);
      if (n < 0 || n > nrec) {
	printf("Position de l'enregistrement non valid.\n\n");
	break;
      }
      printf("Prénom : ");
      scanf("%s", rec.name);
      printf("Nom : ");
      scanf("%s", rec.surname);
      printf("INE : ");
      scanf("%d", &rec.ine);
      rec_write(fd, n, &rec);
      printf("%s %s %08d\n\n", rec.name, rec.surname, rec.ine);
      break;
    }
    case 3: {
      int nrec = rec_count(fd);
      printf("Le fichier contient %d enregistrements.\n\n", nrec);
      break;
    }
    case 4:{
        unsigned ine;
        printf("Veuillez entrez l'ine a chercher : ");
        scanf("%u", &ine);
        if ( rec_search(fd, ine) == -1){
            printf("l'ine donné n'existe pas\n");
            break;
        }
        printf("l'ine donné est posisionné a : %d\n", rec_search(fd,ine));
        break;
    }
    default:
      printf("%d n'est une action connue.\n", a);
    }
  }
  
  return EXIT_SUCCESS;
}

/*
 * Définition des fonctions rec_count, rec_read et rec_write
 */

