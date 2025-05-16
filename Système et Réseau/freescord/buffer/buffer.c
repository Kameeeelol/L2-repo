#include "buffer.h"

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct buffer {
	int fd;             // Descripteur de fichier
	char *data;			// Pointeur vers le tampon mémoire 
	size_t pos;			// Position du prochain octet a lire dans la tampon 0 <= pos <= length
	size_t length;		// Nombre d'octets valide actuellement stocké dans data
	size_t size;		// capacité du tampon 
	int oef;			// Indicatif d’atteinte de la fin de fichier, mis à 1 quand read() retourne 0.
	int ungot;			// Valeur du caractère repoussé, -2 si y'a pas de caractère a repoussé (0-255) sinon
};

buffer *buff_create(int fd, size_t buffsz)
{
	struct buffer *buff = malloc(sizeof(struct buffer));
	if (!buff) return NULL;
	buff->fd = fd;
	buff->size = buffsz;
	buff->data = malloc(buffsz);
	if (!buff->data){
		free(buff->data);
		return NULL;
	}
	buff->length = 0;
	buff->pos = 0;
	buff->oef = 0;
	buff->ungot = -2; 
	return buff;
}

int buff_getc(buffer *b)
{
	if (b->ungot != -2){
		int i = b->ungot;
		b->ungot = -2;
		return i;
	}
	if (b->pos >= b->length){
		ssize_t n = read(b->fd, b->data, b->size);
		if (n < 0) return EXIT_FAILURE;
		if (n == 0){
			b->oef = 1;
			return EOF;
		}
		b->length = n;
		b->pos = 0;
	}
	return b->data[b->pos++];
}

int buff_ungetc(buffer *b, int c)
{
	if (b->ungot != -2 || c == EOF) return EOF;
	b->ungot = c;
	return c;
}

void buff_free(buffer *b)
{
	if (!b) return;
	free(b->data);
	free(b);
}

int buff_eof(const buffer *buff)
{
	return buff->oef && buff->pos >= buff->length && buff->ungot == -2;
}

int buff_ready(const buffer *buff)
{
	return (buff->ungot != -2) || (buff->pos < buff->length);
}

char *buff_fgets(buffer *b, char *dest, size_t size)
{
	if (!b || !dest || size < 2) return NULL;
	int i;
	size_t c = 0;
	while (c+1 < size && (i = buff_getc(b)) != EOF){
		dest[c++] = i;
		if (i == '\n') break; 
	}
	if (c==0) return NULL;
	dest[c] = '\0';
	return dest;
}

char *buff_fgets_crlf(buffer *b, char *dest, size_t size) {
    if (b == NULL || dest == NULL || size == 0) {
        return NULL;  // Vérification de validité des arguments
    }
    size_t i = 0;
    int prev_c = -1;  // Variable pour stocker le caractère précédent
    // Lire des caractères jusqu'à ce qu'on atteigne la fin de la ligne ou du fichier
    while (i < size - 1) {
        int c = buff_getc(b);  // Lire un caractère du buffer
        if (c == -1) {
            // EOF ou erreur de lecture
            if (i == 0) {
                return NULL;  // Si aucun caractère n'a été lu
            }
            break;  // Fin de lecture
        }

        // Stocke le caractère courant
        dest[i++] = (char)c;

        // Si on rencontre la séquence CRLF ("\r\n"), on s'arrête
        if (prev_c == '\r' && c == '\n') {
            break;
        }

        // Met à jour prev_c pour vérifier la séquence "\r\n"
        prev_c = c;
    }

    dest[i] = '\0';  // Ajouter un caractère de fin de chaîne

    return dest;
}
