#include <stdio.h>
#define MAX 1000
int main(void)
{
	char c;
	int gauche = 0, droite = MAX - 1, n;
	do {
		n = (gauche + droite) / 2;
		printf("%d\n", n);
		scanf(" %c", &c);
		if (c == '<') {
			droite = n;
		}
		if (c == '>') {
			gauche = n;
		}
	} while (c != '=');
	return 0;
}
