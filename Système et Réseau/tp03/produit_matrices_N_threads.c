#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef N /* nombre de lignes des matrices */
#define N 10
#endif

void imprime(int matrice[N][N]);
void generation_aleatoire(int matrice[N][N]);
void *idk(void *arg);

int A[N][N], B[N][N], R[N][N];

int main(void) {
  struct timespec tic, toc;
  double duree;
  int i;

  srand(time(NULL));
  generation_aleatoire(A);
  generation_aleatoire(B);
  clock_gettime(CLOCK_REALTIME, &tic);

  pthread_t th[N];
  int j[N];

  for (i = 0; i < N; ++i) {
    j[i] = i;
    pthread_create(&th[i], NULL, idk, &j[i]);
  }

  for (i = 0; i < N; ++i) {
    pthread_join(th[i], NULL);
  }

  clock_gettime(CLOCK_REALTIME, &toc);
  duree = (toc.tv_sec - tic.tv_sec);
  duree += (toc.tv_nsec - tic.tv_nsec) / 1000000000.0;
  printf("durée : %g\n", duree);
  imprime(R);
  return 0;
}

void imprime(int matrice[N][N]) {
  int i, j;
  for (i = 0; i < N; ++i)
    for (j = 0; j < N; ++j)
      printf("%d%c", matrice[i][j], j == N - 1 ? '\n' : '\t');
}

void generation_aleatoire(int matrice[N][N]) {
  int i, j;
  for (i = 0; i < N; ++i)
    for (j = 0; j < N; ++j)
      matrice[i][j] = rand() % 10;
}

void *idk(void *arg) {
  int i = *((int *)arg);
  for (int j = 0; j < N; j++) {
    R[i][j] = 0;
    for (int k = 0; k < N; ++k) {
      R[i][j] += A[i][k] * B[k][j];
    }
  }
  return NULL;
}
