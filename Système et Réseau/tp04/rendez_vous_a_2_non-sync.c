#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COEFF_ATTENTE 100000
#define COEFF_DELTA 5000

void *compteur1(void *);
void *compteur2(void *);
void prendre_du_temps_pour_compter();

int main(void) {
  pthread_t th1, th2;
  sem_t s[2];
  sem_init(&s[0], 0, 1);
  sem_init(&s[1], 0, 0);
  if (pthread_create(&th1, NULL, compteur1, &s) != 0) {
    perror("create th1");
    return 1;
  }
  if (pthread_create(&th2, NULL, compteur2, &s) != 0) {
    perror("create th2");
    return 1;
  }

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  return 0;
}

void *compteur1(void *arg) {
  sem_t *s = arg;
  int i;
  for (i = 0; i <= 100; i = i + 1) {
    sem_wait(s);
    prendre_du_temps_pour_compter();
    printf("a : %d\n", i);
    sem_post(s + 1);
  }
  return NULL;
}

void *compteur2(void *arg) {
  sem_t *s = arg;
  int i;
  for (i = 0; i <= 100; i = i + 1) {
    sem_wait(s + 1);
    prendre_du_temps_pour_compter();
    printf("b : %d\n", i);
    sem_post(s);
  }
  return NULL;
}

void prendre_du_temps_pour_compter() {
  struct timespec ts = {
      .tv_nsec = (COEFF_ATTENTE + (rand() % 10) * COEFF_DELTA) * 1000};
  nanosleep(&ts, NULL);
}
