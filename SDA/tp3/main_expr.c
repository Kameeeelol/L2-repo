#include "binary_tree.h"
#include "BT.h"
#include "item.h"
#include "queue_link.h"
#include "stack_link.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int is_valid_prefix_expression(const char *expr) {
    int operator_count = 0, operand_count = 0;
    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == '+' || expr[i] == '*') {
            operator_count++;
        } else if (expr[i] >= '0' && expr[i] <= '9') {
            operand_count++;
        } else {
            return 0; // Caractère invalide
        }
    }
    // En notation préfixée, le nombre d'opérateurs doit être inférieur au nombre d'opérandes.
    return operand_count > operator_count;
}


link parse(char *expr, int *i){
    if(expr[*i]=='\0') return NULL;
    char c = expr[*i];
    (*i)++;
    link tree = cons_binary_tree(c,NULL,NULL);
    if(c=='+' || c=='*'){
        tree->left = parse(expr, i);
        if (tree->left == NULL) {
            fprintf(stderr, "Erreur : arbre gauche non valide.\n");
            return NULL;
        }
        tree->right = parse(expr, i);
        if (tree->right == NULL) {
            fprintf(stderr, "Erreur : arbre droit non valide.\n");
            return NULL;
        }
    }
    return tree;
}

link parse_expr(char *expr){
    int i = 0;
    return parse(expr, &i);
}

int eval_tree(const link tree){
    char c = get_binary_tree_root(tree);
    if (c>='0' && c<='9') return c-'0';
    if (c=='+') return eval_tree(tree->left)+eval_tree(tree->right);
    if (c=='*') return eval_tree(tree->left)*eval_tree(tree->right);
    return EXIT_FAILURE;
}

void BT_to_expr(link l, char *e, int *i){
    if(is_empty_binary_tree(l)) return;
    e[(*i)++]= get_binary_tree_root(l);
    BT_to_expr(left(l),e,i);
    BT_to_expr(right(l),e,i);
}

char *tree_to_expr(link l){
    char *e = malloc(sizeof(char) * (size_binary_tree(l) + 1));
    if (e == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire échouée.\n");
        return NULL;
    }
    int i = 0;
    BT_to_expr(l,e,&i);
    e[i]='\0';
    return e;
}




int main(int argc, char *argv[] ){
    if (argc != 2) {
        printf("l'expresion aritmétique est attendu en ligne de commande");
        return EXIT_FAILURE;
    }
    char *expr = argv[1];
    if (!is_valid_prefix_expression(expr)) {
        fprintf(stderr, "Erreur : l'expression n'est pas en notation préfixée.\n");
        return EXIT_FAILURE;
    }
    printf("Expression lue : %s\n", expr);
    link tree = parse_expr(expr);
    show_binary_tree(tree,0);
    eval_tree(tree);
    char *expr2 = tree_to_expr(tree);
    if (strcmp(expr, expr2) == 0) {
    printf("les deux expressions sont identiques\n");
    } else {
        printf("les deux expressions ne sont pas identiques\n");
        return EXIT_FAILURE;
    }
    
return EXIT_SUCCESS;
}