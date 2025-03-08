#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HAUTEUR 40
#define LARGEUR 80
#define NUM_ITER_DFT 30
#define DELAI 250000

int ecran1[HAUTEUR][LARGEUR];
int ecran2[HAUTEUR][LARGEUR];
int (*ecran_a_afficher)[HAUTEUR][LARGEUR] = &ecran1;
int (*ecran_a_modifier)[HAUTEUR][LARGEUR] = &ecran2;
void effacer_lignes(int num_lignes);
void *calcul(void *arg);
void *affichage(void *arg);
void frame_prochain();

sem_t sem;
pthread_mutex_t mutex;

/* Attendre delai microsecondes */
void microsleep(int delai);

int main(int argc, char *argv[]) {
  int num_iter = argc > 1 ? atoi(argv[1]) : NUM_ITER_DFT;
  sem_init(&sem, 0, 0);
  pthread_mutex_init(&mutex, NULL);
  pthread_t t_calcul, t_affichage;
  pthread_create(&t_calcul, NULL, calcul, &num_iter);
  pthread_create(&t_affichage, NULL, affichage, &num_iter);
  pthread_join(t_calcul, NULL);
  pthread_join(t_affichage, NULL);
  sem_destroy(&sem);
  pthread_mutex_destroy(&mutex);

  return 0;
  puts("YOU LOSE.\n");
  return 0;
}
void frame_prochain() {
  for (int y = HAUTEUR - 1; y > 0; y--) {
    for (int x = 0; x < LARGEUR; x++) {
      (*ecran_a_modifier)[y][x] = (*ecran_a_afficher)[y - 1][x];
    }
  }
  (*ecran_a_modifier)[0][rand() % LARGEUR] = 1;
}
void *calcul(void *arg) {
  int num_iter = *(int *)arg;
  for (int i = 0; i < num_iter; i++) {
    frame_prochain();
    pthread_mutex_lock(&mutex);
    int(*temp)[HAUTEUR][LARGEUR] = ecran_a_afficher;
    ecran_a_afficher = ecran_a_modifier;
    ecran_a_modifier = temp;
    pthread_mutex_unlock(&mutex);
    sem_post(&sem);
  }
  return NULL;
}

void *affichage(void *arg) {
  int num_iter = *(int *)arg;
  for (int i = 0; i < num_iter; i++) {
    sem_wait(&sem);
    printf("\033[2J");
    for (int y = 0; y < HAUTEUR; y++) {
      for (int x = 0; x < LARGEUR; x++) {
        printf((*ecran_a_afficher)[y][x] ? "*" : " ");
      }
      printf("\n");
    }
    microsleep(10000);
  }
  return NULL;
}

void effacer_lignes(int num_lignes) {
  printf("\033[2K");
  for (; num_lignes > 0; --num_lignes) {
    printf("\033[F");
    printf("\033[2K");
  }
}

void microsleep(int delai) {
  struct timespec ts = {.tv_sec = delai / 1000000,
                        .tv_nsec = 1000 * (delai % 1000000)};
  nanosleep(&ts, NULL);
}
