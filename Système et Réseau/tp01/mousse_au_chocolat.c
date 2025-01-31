#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void casser_chocolat(void);
void faire_fondre_chocolat(void);
void separer_blanc_jaune(void);
void melanger_jaunes_choco(void);
void battre_blancs_neige(void);
void incorporer_blancs_neige(void);

int main(void)
{
	int i;

	casser_chocolat();
	faire_fondre_chocolat();
	for (i = 0; i < 6; ++i)
		separer_blanc_jaune();
	battre_blancs_neige();
	melanger_jaunes_choco();
	incorporer_blancs_neige();

	printf("PID %d : la mousse est terminée !\n", getpid());
}

void casser_chocolat(void)
{
	printf("PID %d : je casse le chocolat.\n", getpid());
	usleep(200000);
	printf("PID %d : le chocolat est cassé !\n", getpid());
}

void faire_fondre_chocolat(void)
{
	printf("PID %d : je fais fondre le chocolat.\n", getpid());
	usleep(1000000);
	printf("PID %d : le chocolat est fondu !\n", getpid());
}

void separer_blanc_jaune(void)
{
	printf("PID %d : je sépare le blanc d'un jaune.\n", getpid());
	usleep(200000);
	printf("PID %d : un blanc est séparé d'un jaune !\n", getpid());
}

void melanger_jaunes_choco(void)
{
	printf("PID %d : je mélange les jaunes et le chocolat.\n", getpid());
	usleep(200000);
	printf("PID %d : le chocolat est mélangé avec les jaunes !\n", getpid());
}

void battre_blancs_neige(void)
{
	printf("PID %d : je bas les blancs en neige.\n", getpid());
	usleep(600000);
	printf("PID %d : les blancs sont battus en neige !\n", getpid());
}

void incorporer_blancs_neige(void)
{
	printf("PID %d : j'incorpore les blancs en neige.\n", getpid());
	usleep(1000000);
	printf("PID %d : les blancs sont incorporés !\n", getpid());
}
