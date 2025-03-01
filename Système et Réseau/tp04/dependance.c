#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define DELAI 100000000

void sleep_alea(void);
void *tache_a();
void *tache_b();
void *tache_c();
void *tache_d();
void *tache_e();

int main(void) {
  srand(time(NULL));
  pthread_t th1, th2, th3, th4, th5;
  sem_t s[4];
  sem_init(&s[0], 0, 0);
  sem_init(&s[1], 0, 0);
  sem_init(&s[2], 0, 0);
  sem_init(&s[3], 0, 0);
  pthread_create(&th1, NULL, tache_a, &s);
  pthread_create(&th2, NULL, tache_b, &s);
  pthread_create(&th3, NULL, tache_c, &s);
  pthread_create(&th4, NULL, tache_d, &s);
  pthread_create(&th5, NULL, tache_e, &s);
  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  pthread_join(th3, NULL);
  pthread_join(th4, NULL);
  pthread_join(th5, NULL);
  sem_destroy(&s[0]);
  sem_destroy(&s[1]);
  sem_destroy(&s[2]);
  sem_destroy(&s[3]);
  return 0;
}

void sleep_alea(void) {
  struct timespec ts = {.tv_nsec = (rand() % 10) * DELAI};
  nanosleep(&ts, NULL);
}

void *tache_a(void *arg) {
  sem_t *s = arg;
  sleep_alea();
  printf("AAAA\n");
  sem_post(s);
  return NULL;
}
void *tache_b(void *arg) {
  sem_t *s = arg;
  sleep_alea();
  sem_wait(s);
  printf("BBBB\n");
  sem_post(s + 1);
  return NULL;
}
void *tache_c(void *arg) {
  sem_t *s = arg;
  sleep_alea();
  sem_wait(s + 1);
  printf("CCCC\n");
  sem_post(s + 2);
  return NULL;
}
void *tache_d(void *arg) {
  sem_t *s = arg;
  sleep_alea();
  sem_wait(s + 2);
  printf("DDDD\n");
  sem_post(s + 3);
  return NULL;
}
void *tache_e(void *arg) {
  sem_t *s = arg;
  sleep_alea();
  sem_wait(s + 3);
  printf("EEEE\n");
  return NULL;
}
