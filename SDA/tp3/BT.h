#ifndef _BT_H
#define _BT_H

#include "item.h"

typedef struct binary_tree {
  item label; /* label of the root */
  struct binary_tree *left; /* NULL if no left subtrees */
  struct binary_tree *right; /* NULL if no right subtree */
} *link;

#endif
