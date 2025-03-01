#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N_DFT 8

/* Retourne la somme 1 + ... + n */
void *somme(void *n);

/* Retourne le produit 1 * ... * n */

void *produit(void *n);

int main(int argc, char *argv[]) {
  int n = argc < 2 ? N_DFT : atoi(argv[1]);
  pthread_t tid_somme, tid_produit;
  long unsigned *s, *p;
  pthread_create(&tid_somme, NULL, somme, &n);
  pthread_create(&tid_produit, NULL, produit, &n);
  pthread_join(tid_somme, (void **)&s);
  pthread_join(tid_produit, (void **)&p);
  printf("n : %d, somme : %lu, produit : %lu\n", n, *s, *p);
  return 0;
}

void *somme(void *n) {
  int *t = (int *)n;
  int i;
  int *res = malloc(sizeof(long unsigned));
  for (i = 1; i <= *t; ++i)
    *res += i;
  return res;
}
void *produit(void *n) {
  int *t = (int *)n;
  int i;
  int *res = malloc(sizeof(long unsigned));
  *res = 1;
  for (i = 1; i <= *t; ++i)
    *res *= i;
  return res;
}
