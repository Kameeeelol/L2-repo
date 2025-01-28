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
    off_t pos = lseek(fd,0,SEEK_CUR);
    off_t size = lseek(fd,0,SEEK_END);
    lseek(fd,pos,SEEK_SET);
    int nb_enregistrement = size / sizeof(rec_t);
    return nb_enregistrement;
}


int rec_read(int fd, int n, rec_t *pr){
    off_t pos = lseek(fd,n*sizeof(rec_t), SEEK_SET);
    if ( pos != n*sizeof(rec_t)) return 0;
    ssize_t bytes_read = read (fd,pr,sizeof(rec_t));
    if( bytes_read != sizeof(rec_t))  return 0;
    return 1;
}


int rec_write(int fd, int n, rec_t *pr){
    off_t pos = lseek(fd,n*sizeof(rec_t), SEEK_SET);
    if ( pos != n*sizeof(rec_t)) return 0;
    ssize_t bytes_read = write(fd,pr,sizeof(rec_t));
    if( bytes_read != sizeof(rec_t))  return 0;
    return 1;
}

int rec_search(int fd, unsigned ine){
    int nb = rec_count(fd);
    for( int i=0; i<nb; i++){
        rec_t rec;
        rec_read(fd,i,&rec);
        if(rec.ine == ine) return i;
    }
    return -1;
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
    case 4: {
        unsigned ine;
        printf("Veuillez entez l'ine de l'étudiant que vous rechercher : ");
        scanf("%u",&ine);
        int n = rec_search(fd,ine);
        if (n==-1){
            printf("Le fichier ne contient pas l'étudiant que vous rechercher\n");
            break;
        }
        rec_t rec;
        rec_read(fd, n, &rec);
        printf("%s %s %08d\n\n", rec.name, rec.surname, rec.ine);
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