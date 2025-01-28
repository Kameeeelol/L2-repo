#ifndef _ITEM_H
#define _ITEM_H

#include <string.h>

typedef char *item;

#define key(A) (A)
#define less(A, B) (strcmp(A, B) < 0)
#define eq(A, B) (strcmp(A, B) == 0)
#define exch(A, B) { item tmp = A; A = B; B = tmp; }
#define display(A) { printf("%s ", key(A)); }
#define destroy(A)

#endif
