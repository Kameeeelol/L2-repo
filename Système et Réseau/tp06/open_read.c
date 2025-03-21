#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSZ 256
int main(void)
{
	int fd = open("lol.txt", O_RDONLY);
	ssize_t n; /* signed size type pour pouvoir accueillir -1 */


	if (fd < 0) {
		perror("open");
		return 1;
	}

	printf("Descripteur de fichier : %d\n", fd);
	
	char buffer[20];
	ssize_t bytes_reads = read(fd,buffer,20);
	buffer[bytes_reads]='\n';
	write(1,buffer,bytes_reads+1);

	close(fd);
	return 0;
}
