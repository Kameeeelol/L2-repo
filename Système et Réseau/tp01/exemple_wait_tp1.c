#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>

void do_something(pid_t p);

int main(void)
{
	pid_t p;
	srand(time(NULL));
	switch (fork()) {
	case -1: perror("fork"); return 1;
	case  0: 
		 printf("Je suis né ! Je m'appelle %d\n", getpid());
		 do_something(getpid());
		 printf("%d: Je vais bientôt mourir.\n", getpid());
		 return 0;
	default:
		switch (fork()) {
		case -1: perror("fork"); return 1;
		case  0: 
			printf("Je suis né ! Je m'appelle %d\n", getpid());
		 	do_something(getpid());
			printf("%d: Je vais bientôt mourir.\n", getpid());
			return 0;
		}
	}
	do_something(getpid());
	
	while ( (p=wait(NULL)) != -1){
		printf("%d à terminé.\n", p);
	}
	// p = wait(NULL);
	//printf("%d a terminé\n", p);
	//p = wait(NULL);
	//printf("%d a terminé\n", p);
	// p = wait(NULL);
	// printf("%d a terminé\n", p);

	printf("Tout le monde a terminé.\n");
	printf("%d: Je vais bientôt mourir.\n", getpid());
	return 0;
}

void do_something(pid_t p)
{
	usleep(100000 + rand() % 100000);
	printf("%d: Roupillon... euh... travail terminé\n", p);
}
