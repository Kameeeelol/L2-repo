#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSZ 256
int main(void)
{
	int fd1 = open("lol1", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	int fd2 = open("lol2", O_CREAT | O_WRONLY | O_APPEND, 0600);
	if (fd1 < 0 || fd2 < 0) {
		perror("open");
		return 2;
	}
	char lol[6] = "lol !\n";
	write(fd1, lol, 6);
	write(fd2, lol, 6);
	close(fd1);
	close(fd2);
	return 0;
}
