#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>


int main (int argc, char *argv[]){
	
	char *path;

	if ( argc != 2 ){
		path = "./";	
	}else{
		path = argv[1];
	}

	switch (fork()){
	case -1 :
		perror("fork");
		return 1;
	case 0 :
		execlp("ls", "ls", path, NULL);
		perror("exec");
		return 1;
	}
	wait(NULL);

	switch (fork()){
	case -1 :
		perror("fork");
		return 1;
	case 0 :
		execlp("du", "du","-sh", path, NULL);
		perror("exec");
		return 1;
	}
	
	wait(NULL);

	execlp("df", "df", path, NULL);
	perror("exec");
	return 1;
	
	return 0;
}

