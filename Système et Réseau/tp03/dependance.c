#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define DELAI 100000000

void sleep_alea(void);
void tache_a();
void tache_b();
void tache_c();
void tache_d();
void tache_e();

int main(void)
{
	srand(time(NULL));
	tache_a();
	tache_b();
	tache_c();
	tache_d();
	tache_e();
	return 0;
}

void sleep_alea(void)
{
	struct timespec ts = { .tv_nsec = (rand() % 10) * DELAI };
	nanosleep(&ts, NULL);
}

void tache_a() { sleep_alea(); printf("AAAA\n"); }
void tache_b() { sleep_alea(); printf("BBBB\n"); }
void tache_c() { sleep_alea(); printf("CCCC\n"); }
void tache_d() { sleep_alea(); printf("DDDD\n"); }
void tache_e() { sleep_alea(); printf("EEEE\n"); }
