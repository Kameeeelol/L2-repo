#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define N 5
#define COEFF_ATTENTE 50000
pthread_mutex_t mutex;
// sem_t *sem; // sem_t sem pour linux
sem_t sem;
int verif = 0;

/* attendre delai microsecondes */
void microsleep(int delai);

void work(void);

void *chiffres_et_alphabet(void *);

int main(void) {
  int i, nums[N];
  pthread_t th[N];
  srand(time(NULL));
  pthread_mutex_init(&mutex, NULL);
  // sem = sem_open("/sem", O_CREAT, 0644, 0); // sem_init(&sem,0,0) pour linux
  sem_init(&sem, 0, 0);

  for (i = 0; i < N; ++i) {
    nums[i] = i;
    if (pthread_create(&th[i], NULL, chiffres_et_alphabet, &nums[i]) != 0) {
      perror("create");
      return 1;
    }
  }
  for (i = 0; i < N; ++i)
    pthread_join(th[i], NULL);
  pthread_mutex_destroy(&mutex);
  // sem_close(sem); // sem_destroy(&sem); pour linux
  // sem_unlink("/sem");
  sem_destroy(&sem);
  return 0;
}

void *chiffres_et_alphabet(void *arg) {
  int i = *((int *)arg);
  printf("%d\n", i);
  pthread_mutex_lock(&mutex);
  verif++;
  pthread_mutex_unlock(&mutex);
  if (verif == N) {
    sem_post(&sem); // sem_post(&sem) pour linux
  }
  sem_wait(&sem); // sem_wait(&sem) pour linux
  printf("%c\n", 'A' + i);
  sem_post(&sem); // sem_post(&sem) pour linux

  return NULL;
}

void work(void) { microsleep((rand() % 10) * COEFF_ATTENTE); }

void microsleep(int delai) {
  struct timespec ts = {.tv_sec = delai / 1000000,
                        .tv_nsec = 1000 * (delai % 1000000)};
  nanosleep(&ts, NULL);
}
