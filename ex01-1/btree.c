#include <assert.h>

#include "btree.h"


struct btree {
	size_t size;
	btree_node* node;
};

struct btree_node {
	btree_node* left;
	btree_node* right;
	int value;
};


btree* btree_create(void) {
	btree* tree =  malloc(sizeof(btree));

	if (tree == NULL) {
		return NULL;
	}
	tree->node = NULL;
	tree->size = 0;

	return tree;
}


void btree_destroy(btree* t) {
	if (t->node == NULL) {
		free(t);
	} else {
		btree_remove_node(t->node);
	}
}

void btree_remove_node(btree_node* node) {
	if (node->left != NULL) {
		btree_remove_node(node->left);
	}
	if (node->right != NULL) {
		btree_remove_node(node->right);
	}
	if (node->left == NULL && node->right == NULL) {
		free(node);
	}
}

void btree_insert(btree* t, int d) {

	printf("inserting %d\n", d);
	btree_node* newnode = malloc(sizeof(btree_node));
	newnode->value = d;
	newnode->left = newnode->right = NULL;

	// if there's no root node, create one
	if (t->node == NULL) {
		t->node = newnode;
		t->size++;
	} else {

		btree_node* currentnode = t->node;
		while (!btree_contains(t, d)) {
			if (currentnode->left == NULL || currentnode->right == NULL) {
				if (d < currentnode->value) {
					currentnode->left = newnode;
				} else {
					currentnode->right = newnode;
				}
				t->size++;
				printf("created root node with value %d\n", newnode->value);
				return;
			} else {
			// there is a root node, follow kids
				if (d < currentnode->value) {
					currentnode = currentnode->left;
				}
				if (d > currentnode->value) {
					currentnode = currentnode->right;
				}
			}
		}
	}
}

btree_node* node_minimum(btree_node* node) {
	btree_node* currentnode = node;
	while (currentnode->left != NULL) {
		currentnode = currentnode->left;
	}
	return currentnode;
}

btree_node* node_maximum(btree_node* node) {
	btree_node* currentnode = node;
	while (currentnode->right != NULL) {
		currentnode = currentnode->right;
	}
	return currentnode;
}

void btree_remove(btree* t, int d) {
	btree_node* parentnode = t->node;
	while(btree_contains(t, d)) {
		if (parentnode->value == d) {
			btree_node* minimum = node_minimum(parentnode->right);
			parentnode->value = minimum->value;
			t->size--;
			if (minimum->right != NULL) {
				minimum = minimum->right;
				free(minimum->right);
			} else {
				minimum = NULL;
				free(minimum);
			}
			return;
		}

		if (d < parentnode->value) {
			parentnode = parentnode->left;
		} else {
			parentnode = parentnode->right;
		}
	}
}


int btree_minimum(const btree* t) {
	btree_node* currentnode = t->node;
	while (currentnode->left != NULL) {
		currentnode = currentnode->left;
	}
	return currentnode->value;
}


int btree_maximum(const btree* t) {
	btree_node* currentnode = t->node;
	while (currentnode->right != NULL) {
		currentnode = currentnode->right;
	}
	return currentnode->value;
}


bool btree_contains(const btree* t, int d) {
	printf("looking for %d in tree\n", d);
	btree_node* currentnode = t->node;
	if (currentnode == NULL) {
		printf("currentnode is NULL - nothing can be found\n");
		return false;
	}
	while(true) {
		printf("entering search loop for %d\n", d);
		//assert(currentnode->value);
		if (currentnode->value == d) {
			printf("found node %d\n", currentnode->value);
			return true;
		} else if (d < currentnode->value) {
			printf("looking left\n");
			if (currentnode->left == NULL) {
				return false;
			} else {
				currentnode = currentnode->left;
			}
		} else if (d > currentnode->value) {
			printf("looking right\n");
			if (currentnode->right == NULL) {
				return false;
			} else {
				currentnode = currentnode->right;
			}
		}
	}
	return false;
}

size_t btree_size(const btree* t) {
	return t->size;
}

void btree_print_node(btree_node* node, FILE* out) {

	fputs("(", out);

	if(node->left) {
		btree_print_node(node->left, out);
		fputs(", ", out);
	}

	fprintf(out, "%d", node->value);

	if(node->right) {
		fputs(", ", out);
		btree_print_node(node->right, out);
	}

	fputs(")", out);

}

void btree_print(const btree* t, FILE* out) {
	btree_node* currentnode = t->node;
	if (currentnode == NULL) {
		fputs("(NIL)", out);
	} else {
		btree_print_node(currentnode, out);
	}

	fprintf(out, ": %zd\n", t->size);
}

