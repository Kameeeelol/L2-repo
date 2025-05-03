#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "sys_utils.h"

ssize_t exact_read(int fd, void *buf, size_t nbytes)
{
	ssize_t n;
	size_t total = 0;
	while (total < nbytes) {
		n = read(fd, (char *) buf + total, nbytes - total);
		if (n == -1) /* error */
			return n;
		if (n == 0) /* end of file */
			break;
		total += n;
	}
	return total;
}

void perror_exit(const char *err_mess)
{
	perror(err_mess);
	exit(2);
}

void *malloc_or_exit(size_t nb_octets)
{
	void *res = malloc(nb_octets);
	if (res == NULL)
		perror_exit("malloc");
	return res;
}

void vider_buffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

uint16_t saisir_entre_bornes(uint16_t borne_inf, uint16_t borne_sup)
{
	uint16_t rep_util;
	int stat;
	do {
		printf("Votre réponse : ");
		stat = scanf("%" SCNu16, &rep_util);
		if (stat < 1)
			vider_buffer();
	} while (stat < 1 || rep_util < borne_inf || rep_util > borne_sup);
	return rep_util;
}
