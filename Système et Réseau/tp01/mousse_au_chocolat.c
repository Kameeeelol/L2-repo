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
    pid_t c1, c2, c3;

    c1 = fork();
    if (c1 == 0) {
        // Processus des œufs : séparation + battage des blancs
        for (int i = 0; i < 6; ++i) {
            if (fork() == 0) {
                separer_blanc_jaune();
                return 0;
            }
        }
        while (wait(NULL) != -1); // Attend les 6 séparations
        battre_blancs_neige();     // 0.6s (commence à 0.2s, fin à 0.8s)
        return 0;
    }

    c2 = fork();
    if (c2 == 0) {
        // Processus du chocolat : cassage + fonte
        casser_chocolat();       // 0.2s
        faire_fondre_chocolat(); // 1.0s (commence à 0.2s, fin à 1.2s)
        return 0;
    }

    c3 = fork();
    if (c3 == 0) {
        // Mélange des jaunes dès que les séparations sont prêtes (0.2s)
        waitpid(c1, NULL, 0);    // Attend seulement les séparations (0.2s)
        melanger_jaunes_choco(); // 0.2s (commence à 0.2s, fin à 0.4s)
        return 0;
    }

    // Le processus principal attend :
    waitpid(c2, NULL, 0); // Attend le chocolat fondu (1.2s)
    waitpid(c3, NULL, 0); // Attend le mélange jaunes (0.4s)
    incorporer_blancs_neige(); // 1.0s (commence à 1.2s, fin à 2.2s)

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
