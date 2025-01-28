#include <stdlib.h>
#include <stdio.h>
#include "bst.h"
#include "binary_tree.h"

link search_BST(link tree, item x) {
	
	if (tree == NULL) return NULL;
	
	if (get_binary_tree_root(tree) > x) {
		return search_BST(tree->left, x);
	}
	
	if (get_binary_tree_root(tree) < x) {	
		return search_BST(tree->right, x);
	}
	
	return tree;
	
}

link insert_BST(link tree, item x) {

	if (tree == NULL) return cons_binary_tree(x, NULL, NULL);
	
	if (less(x, get_binary_tree_root(tree))) {
		tree->left = insert_BST(tree->left, x);
	}
			
	else {
		tree->right = insert_BST(tree->right, x);
	}
	
	return tree;
}

link insert_BST_iter(link tree, item x) {

	link tmp = tree, p;
	
	if (tree == NULL) return cons_binary_tree(x, NULL, NULL);
	
	while (tree != NULL) {
	
		p = tree;
		tree = (less(x, get_binary_tree_root(tree))) ? tree->left : tree->right;
	}
	
	if (less(x, get_binary_tree_root(p))) {
		p->left = cons_binary_tree(x, NULL, NULL);
	}
	
	else {
		p->right = cons_binary_tree(x, NULL, NULL);
	}
	
	return tmp;
}

link select_BST(link h, int k) {

	if (h==NULL) return NULL;
	
	if (k == size_binary_tree(h->left)) return h;
	
	if (k < size_binary_tree(h->left)) return select_BST(h->left, k);

	return select_BST(h->right, k-1-size_binary_tree(h->left));

}

link rotate_right(link h) {
	
	if (h==NULL) return NULL;
	
	link tmp1 = h;
	link tmp2 = (h->left)->right;
		
	h = h->left;
	h->right = tmp1;
	(h->right)->left = tmp2;
	
	
	return h;
}


link rotate_left(link h) {
	
	if (h==NULL) return NULL;
	
	link tmp1 = h;
	link tmp2 = (h->right)->left;
		
	h = h->right;
	h->left = tmp1;
	(h->left)->right = tmp2;
	
	
	return h;
}


link insert_BST_root(link tree, item x) {
	
	if (tree == NULL) return cons_binary_tree(x, NULL, NULL);

	link h;
	
	if (less(x, get_binary_tree_root(tree))) {
		h = insert_BST_root(tree->left, x);
		h = rotate_right(tree); 
	}
	
	else {
		h = insert_BST_root(tree->right, x);
		h = rotate_left(tree);
	}		
	
	return h;
}


link insert_BST_root_it(link tree, item x) {
	
	if (tree == NULL) return cons_binary_tree(x, NULL, NULL);
	
	while (tree != NULL) {
		
		if (less(x, get_binary_tree_root(tree))) {
			if (tree->left == NULL) {
				tree->left = cons_binary_tree(x, NULL, NULL);
				tree = rotate_right(tree);
				break;
			}
			tree = rotate_right(tree);
		}
		
		else {
			if (tree->right == NULL) {
				tree->right = cons_binary_tree(x, NULL, NULL);
				tree = rotate_left(tree);
				break;
			}
			tree = rotate_left(tree);
		}
	
	}
	
	return tree;
}


link partition_BST(link tree, int k) {
	
	if (tree == NULL) return NULL;

	if (k < size_binary_tree(tree->left)) {
		tree->left = partition_BST(tree->left, k);
		tree = rotate_right(tree);
	}	
	
		
	if (k > size_binary_tree(tree->left)){
		tree->right = partition_BST(tree->right, k-1-size_binary_tree(tree->left));
		tree = rotate_left(tree);
	}
	
	return tree;

}

link delete_node_BST(link h, item v) {
	
	if (h == NULL) return NULL;
	
	link f = h, p = h, tmp;
	int i=0;
	
	while (h !=  NULL) {
		
		p = h;
		
		if (get_binary_tree_root(h) > v) {
			h = h->left;
			i=0;
			continue;
		}
	
		if (get_binary_tree_root(h) < v) {	
			h = h->right;
			i=1;
			continue;
		}
	
		break;
	}
	
	if (h == NULL)	return NULL;
	
	if (!h->left && !h->right) {
		free(h); 
		h=NULL;	
		return f;
	}
	
	if (!h->left) {
		if (i==0) {
			p->left = h->right;
		}
		else {
			p->right = h->right;
		}
		
		free(h); 
		h=NULL;	
		return f;
	}
	
	if (!h->right) {
		if (i==0) {
			p->left = h->left;
		}
		else {
			p->right = h->left;
		}
		
		free(h); 
		h=NULL;	
		return f;
	}
	
	
	tmp = partition_BST(h->right, 0);
	
	tmp->left = h->left;
	
	if (i==0) {
		p->left = tmp;
	}
	else {
		p->right = tmp;
	
	}
	
	
	free(h);
	h=NULL;	
	
	return f;
}


link balance_BST(link h) {

	if (!h) return NULL;
	
	if (size_binary_tree(h) <= 2) return h;
	
	link tree = partition_BST(h, size_binary_tree(h)/2);
	
	tree->left = balance_BST(h->left);
	
	tree->right = balance_BST(h->right);
	
	return tree;
}

link join_BST(link h, link k) {
	
	if (k==NULL) return h;
	if (h==NULL) return k;
	
	k = insert_BST_root(k, get_binary_tree_root(h));
	
	k->left = join_BST(h->left, k->left);
	k->right = join_BST(h->right, k->right);

	
	delete_binary_tree(&h);

	return k;

}