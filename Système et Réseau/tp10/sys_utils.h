#ifndef _SYS_UTILS_H
#define _SYS_UTILS_H
#include <stdint.h>
#include <unistd.h>

/** Allouer nbytes octets et renvoie un pointeur sur la mémoire allouée.
 * En cas d'échec, affiche un message d'erreur et met fin au programme avec
 * statut de sortie 2. */
void *malloc_or_exit(size_t nbytes);

/** Lire exactement nbytes octets (si besoin en appelant plusieurs fois read)
 * dans le ficher de descripteur fd ou bien jusqu'à la fin du fichier et les
 * copier dans buf.
 * Retourne le nombre d'octets lus ou -1 en cas d'erreur. */
ssize_t exact_read(int fd, void *buf, size_t nbytes);

/** Utiliser la fonction perror avec l'argument err_mess et mettre fin au
 * programme avec statut de sortie 2. */
void perror_exit(const char *err_mess);

/** Vider le tampon associé à l'entrée standard */
void vider_buffer();

/** Retourner un entier positif saisi par l'utilisateur, sur deux octets, compris
 * entre borne_inf (incluse) et borne_sup (incluse) */
uint16_t saisir_entre_bornes(uint16_t borne_inf, uint16_t borne_sup);

#endif /* ifndef _SYS_UTILS_H */
