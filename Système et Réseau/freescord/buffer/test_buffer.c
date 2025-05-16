/* test_buffer.c
 * Programme de test pour la bibliothèque d'entrées bufferisées (buffer.c/h)
 * Usage :
 *   ./test_buffer [mode]
 * Modes disponibles :
 *   lf   : lecture avec buff_fgets (détection de LF)
 *   crlf : lecture avec buff_fgets_crlf (détection de CRLF)
 * Sans argument, lit caractère par caractère et teste buff_getc / buff_ungetc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "buffer.h"

int main(int argc, char **argv) {
    buffer *buf = buff_create(STDIN_FILENO, 16);
    if (!buf) {
        perror("buff_create");
        return EXIT_FAILURE;
    }

    if (argc >= 2 && strcmp(argv[1], "lf") == 0) {
        char line[256];
        printf("=== Test buff_fgets (LF) ===\n");
        while (buff_fgets(buf, line, sizeof(line))) {
            printf("[LF] %s", line);
        }
    } else if (argc >= 2 && strcmp(argv[1], "crlf") == 0) {
        char line[256];
        printf("=== Test buff_fgets_crlf (CRLF) ===\n");
        while (buff_fgets_crlf(buf, line, sizeof(line))) {
            printf("[CRLF] %s", line);
        }
    } else {
        printf("=== Test buff_getc / buff_ungetc ===\n");
        int c;
        while ((c = buff_getc(buf)) != EOF) {
            putchar(c);
            if (c == 'X') {
                printf("\n(Pousse 'X' en arrière)\n");
                if (buff_ungetc(buf, 'X') == EOF) {
                    fprintf(stderr, "buff_ungetc a échoué\n");
                } else {
                    printf("Lecture après ungetc: %c\n", buff_getc(buf));
                }
            }
        }
    }

    buff_free(buf);
    return EXIT_SUCCESS;
}
