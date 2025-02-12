#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void)
{
	switch (fork()) {
	case -1:
		perror("fork");
		return 1;
	case 0:
		execlp("cp", "cp", "exemple_execlp.c", "exemple_copie.c",  NULL);
		perror("exec: ");
		return 1;
	}
	wait(NULL);
	printf("Commande ls terminée.\n");
	return 0;
}
