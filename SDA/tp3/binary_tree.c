#include "binary_tree.h"
#include "BT.h"
#include "item.h"
#include "queue_link.h"
#include "stack_link.h"
#include <stdlib.h>
#include <stdio.h>
#define N 20


link empty_tree(){
    return NULL;
}

link cons_binary_tree(item i, const link d, const link g){
    link tree = (link)malloc(sizeof(struct binary_tree));
    if(tree == NULL){
        printf("l'allocation mémoire a échouée");
        return NULL;
    }
    tree->label = i;
    tree->left = g;
    tree->right = d;
    return tree;
}

void delete_binary_tree(link *tree){
    free(*tree);
    delete_binary_tree(&((*tree)->left));
    delete_binary_tree(&((*tree)->right));
}

link left(const link g){
    return g->left;
}

link right(const link d){
    return d->right;
}

item get_binary_tree_root(const link tree){
    return tree->label;
}

int is_empty_binary_tree(const link tree){
    return tree == NULL;
}

int size_binary_tree(const link tree){
    return 1 + size_binary_tree(tree->right) + size_binary_tree(tree->left);
}

int height_binary_tree(const link tree){
    int heightG = height_binary_tree(tree->left);
    int heightD = height_binary_tree(tree->right);
    return 1 + (heightG>heightD ? heightG : heightD);
}

void print_label(const link tree){
    display(tree->label);
}

void show_binary_tree(const link tree, int t){
    show_binary_tree(tree->right, t+1);
    for (int i = 0; i < t; i++) {
        printf("\t");
    }
    printf("%d\n", tree->label);
    show_binary_tree(tree->left, t+1);
}

void traverse_inorder_binary_tree(link tree, void (*c)(link)){
    if(tree==NULL) return;
    traverse_inorder_binary_tree(tree->left,c);
    c(tree);
    traverse_inorder_binary_tree(tree->right,c);
}

void traverse_preorder_binary_tree(link tree, void (*c)(link)){
    if(tree==NULL) return;
    c(tree);
    traverse_preorder_binary_tree(tree->left,c);
    traverse_preorder_binary_tree(tree->right,c);
}

void traverse_postorder_binary_tree(link tree, void (*c)(link)){
    if(tree==NULL) return;
    traverse_postorder_binary_tree(tree->left,c);
    traverse_postorder_binary_tree(tree->right,c);
    c(tree);
}

void traverse_preorder_it_BT(link tree, void (*c)(link)){
    struct stack s = {N, NULL};
    push_stack(&s,tree);
    while (s.contents!=NULL){
        link courant = pop_stack(&s);
        c(courant);
        if(courant->right!=NULL) push_stack(&s, courant->right);
        if(courant->left!=NULL) push_stack(&s, courant->left);
    }
}

void traverse_inorder_it_BT(link tree, void (*c)(link)){
    struct stack s ={N,NULL};
    link courant = tree;
    while (courant!=NULL || s.contents != NULL){
        while(courant!=NULL){
            push_stack(&s,courant);
            courant = courant->left;
        }
        courant = pop_stack(&s);
        c(courant);
        courant = courant->right;
    }
}

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