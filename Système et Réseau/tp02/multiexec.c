#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>


int main (int argc, char *argv[]){
	
	if (argc < 3){
		perror("argc");
		return 1;
	}

	int nb = atoi(argv[1]);
	char *command = argv[2];
	char **paths = &argv[2];
	int status;
	
	for (int i=0; i<nb; i++){
		switch (fork()){
			case -1:
				perror("fork");
				return 1;
			case 0:
				execvp(command, paths);
				perror("execvp");
				return 1;
		}
		wait(&status);
       	}

	if (WIFEXITED(status)){
		exit(WEXITSTATUS(status));
	}else if (WIFSIGNALED(status)){
		exit(128 + WTERMSIG(status));
	}else{
		exit(EXIT_FAILURE);
	}

	return 0;

}
