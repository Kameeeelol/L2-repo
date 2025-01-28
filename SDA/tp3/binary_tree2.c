#include "binary_tree.h"
#include "stack_link.h"
#include "queue_link.h"
#include <stdlib.h>
#include <stdio.h>

/** "Creates" an empty tree and returns its address */
link empty_tree() {
  return NULL;
}

/* Constructs a binary tree from a label and two binary trees */
/* (allocates memory and returns the allocated block's address) */
link cons_binary_tree(item x, const link pt_l, const link pt_r) {
  link pt = malloc(sizeof(struct binary_tree));
  pt->label = x;
  pt->left = pt_l;
  pt->right = pt_r;
  return pt;
}

/* Frees all memory allocated to a tree and its subtrees... */
void delete_BT(link pt) {
  if (NULL != pt) {
    delete_BT(left(pt));
    delete_BT(right(pt));
    free(pt);
  }
}
/* ... and sets pointeur to deleted binary tree to NULL */
void delete_binary_tree(link *pt) {
  delete_BT(*pt);
  *pt = NULL;
}

/** Returns the left subtree of a tree */
/* (requires that the tree be non-empty) */
link left(const link pt) {
  return pt->left;
}

/** Returns the right subtree of a tree */
/* (requires that the tree be non-empty) */
link right(const link pt) {
  return pt->right;
}

/** Returns the label of the root of a tree */
/* (requires that the tree be non-empty) */
item get_binary_tree_root(const link pt) {
  return pt->label;
}

/** Tests a tree for emptyness */
int is_empty_binary_tree(const link pt) {
  return (NULL == pt);
}

/** Computes recursively and returns the size of a tree */
int size_binary_tree(const link tree) {
  if (tree == NULL) return 0;
  return size_binary_tree(tree->left) + size_binary_tree(tree->right) + 1;
}

int max(int x, int y) {
  if (x > y) { return x; } else { return y; }
}

/** Computes recursively and returns the height of a tree */
int height_binary_tree(const link tree) {
  if (tree == NULL) return 0;
  return 1 + max(size_binary_tree(tree->left), size_binary_tree(tree->right));
}

/** Prints the label of a node with the appropriate shift */
void print_node_shift(item s, int shift) {
  int i;
  for (i = 0; i < shift; i++) printf("    ");
  display(s);
  printf("\n");
}
/** Prints a tree (rotated by \pi/2) */
void show_binary_tree(const link tree, int shift) {
  if (NULL == tree) return;
  show_binary_tree(tree->right, shift + 1);
  print_node_shift(get_binary_tree_root(tree), shift);
  show_binary_tree(tree->left, shift + 1);
}

/*********** RECURSIVE TRAVERSALS ****************/
/** Displays information attached to the root */
void print_label(link h) {
  display(get_binary_tree_root(h));
}

/** Traverses recursively a tree according to inorder */
void traverse_inorder_binary_tree(link h, void (*visit)(link)) {
  if (h == NULL) return;
  traverse_inorder_binary_tree(h->left, visit);
  (*visit)(h);
  traverse_inorder_binary_tree(h->right, visit);
}

/** Traverses recursively a tree according to preorder */
void traverse_preorder_binary_tree(link h, void (*visit)(link)) {
  if (h == NULL) return;
  (*visit)(h);
  traverse_preorder_binary_tree(h->left, visit);
  traverse_preorder_binary_tree(h->right, visit);
}

/** Traverses recursively a tree according to postorder */
void traverse_postorder_binary_tree(link h, void (*visit)(link)) {
  if (h == NULL) return;
  traverse_postorder_binary_tree(h->left, visit);
  traverse_postorder_binary_tree(h->right, visit);
  (*visit)(h);
}

/******* ITERATIVE TRAVERSALS (PREORDER, INORDER AND LEVEL-ORDER)  ******/
/** Traverses iteratively a tree according to preorder (using a stack) */
void traverse_preorder_it_BT(link h, void (*visit)(link)) {
  struct stack * st = init_stack(size_binary_tree(h)+1);
  push_stack(st, h);
  while (!is_empty_stack(st)) {
    (*visit)(h = pop_stack(st));
    if (h->right != NULL) push_stack(st, h->right);
    if (h->left != NULL) push_stack(st, h->left);
  }
  delete_stack(&st);
}

/** Traverses iteratively a tree according to inorder (using a stack) */
void traverse_inorder_it_BT(link h, void (*visit)(link)) {
    /* Initialization of the pile */
    struct stack * st = init_stack(size_binary_tree(h)+1);
    int finished = 0;
    /* finished will be set to 1 when the stack is empty AND h is NULL */
    while (!finished) {
        if (h != NULL) {
        /* Ones reaches the left most node of the current node */
            push_stack(st, h);
            h = h->left;
        }
        else {
        /* One backtracks from the empty subtree and visit the node at the top of the stack  */
            if (!is_empty_stack(st)) {
                h = pop_stack(st);
                (*visit)(h);
                /* The node and its left subtree have been visited */
                /* It is the right subtree's turn */
                h = h->right;
            }
            else {
                finished = 1;
            }
        }
    }
}

/** Traverses iteratively a tree according to level-order (using a queue) */
void traverse_level_BT(link h, void (*visit)(link)) {
    struct queue * q = init_queue();
    enqueue(q, h);
    while (!is_empty_queue(q)) {
        (*visit)(h = dequeue(q));
        if (h->left != NULL) enqueue(q, h->left);
        if (h->right != NULL) enqueue(q, h->right);
    }
    delete_queue(&q);
}
