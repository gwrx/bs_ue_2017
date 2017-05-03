#include "btree.h"


struct btree {
	btree_node *node;
	btree *left;
	btree *right;
	size_t size;
};


struct btree_node {
	int value;
};


btree* btree_create() {
	btree *tree = (btree*)malloc(sizeof(btree));
	tree->node = NULL;
	tree->left = tree->right = NULL;
	tree->size = 0;
	return tree;
}


/// Destroys and deallocates all memory for the given tree 't'
void btree_destroy(btree* t) {
	if (t->node == NULL) {
		btree_destroy(t->left);
		btree_destroy(t->right);
		free(t);
		if (t->left != NULL) {
			t->left = NULL;
			free(t->left);
		}
		if (t->right != NULL) {
			t->right = NULL;
			free(t->right);
		}
		t = NULL;
	}
}


/// Inserts the given number 'd' into tree 't'
/// if the number is already in 't', no changes are made
void btree_insert(btree* t, int d) {
	if (!btree_contains(t, d)) {
		t->size++;
		if (t->node == NULL) {
			//root case
			t->node = (btree_node*)malloc(sizeof(btree_node));
			t->node->value = d;
			t->left = t->right = NULL;
		} else {
			if (t->node->value > d) {
				if (t->left == NULL) {
					t->left = btree_create();
				}
				btree_insert(t->left, d);
			} else {
				if (t->right == NULL) {
					t->right = btree_create();
				}
				btree_insert(t->right, d);
			}
		}
	}
}


/// Removes the given number 'd' from tree 't'
void btree_remove(btree* t, int d) {
	if (btree_contains(t, d)) {
		t->size--;
		if (t->node->value > d) {
			btree_remove(t->left, d);
			if ((t->left->left == NULL) && (t->left->right == NULL)) {
				t->left = NULL;
			}
		} else if (t->node->value < d) {
			btree_remove(t->right, d);
			if ((t->right->left == NULL) && (t->right->right == NULL)) {
				t->right = NULL;
			}
		} else {
			//found node
			if ((t->left == NULL) && (t->right == NULL)) {
				t = NULL;
			} else if ((t->left == NULL) || (t->right == NULL)) {
				if (t->left != NULL) {
					//free(t->node);
					//free(t);
					t = t->left;
					t->left = NULL;
				} else {
					//free(t->node);
					//free(t);
					t = t->right;
					t->right = NULL;
				}
			} else {
				//replace node, with max of left subtree
				int max_node = btree_maximum(t->left);
				t->node->value = max_node;
				btree_remove(t->left, max_node);
			}
		}
	}
}


/// Returns the smallest number in tree 't'
int btree_minimum(const btree* t) {
	if (t->left == NULL) {
		return t->node->value;
	} else {
		return btree_minimum(t->left);
	}
}


/// Returns the largest number in tree 't'
int btree_maximum(const btree* t) {
	if (t->right == NULL) {
		return t->node->value;
	} else {
		return btree_maximum(t->right);
	}
}


/// Returns true if the given tree 't' contains 'd', false otherwise
bool btree_contains(const btree* t, int d) {
	if (t->node != NULL) {
		if (t->node->value > d) {
			if (t->left != NULL) {
				return btree_contains(t->left, d);
			}
		} else if (t->node->value < d) {
			if (t->right != NULL) {
				return btree_contains(t->right, d);
			}
		} else {
			return true;
		}
	}
	return false;
}


/// Returns the number of elements of the given btree 't'
/// NOTE: should complete in O(1) time
size_t btree_size(const btree* t) {
	return t->size;
}


/// Prints the given btree 't' to the supplied output stream 'out'
/// output format: ([LEFT], VAL, [RIGHT]) : [SIZE]
/// example empty: ( NIL ) : 0
/// example 3,4,7 in a balanced tree: ((3), 4, (7)) : 3
void btree_print(const btree* t, FILE* out) {
	if (t->node == NULL) {
		fprintf(out, "NIL\n");
	} else {
		if (t->left != NULL) {
			fprintf(out, "(");
			btree_print(t->left, out);
			fprintf(out, ")");
		}

		fprintf(out, " %d ", t->node->value);

		if (t->right != NULL) {
			fprintf(out, "(");
			btree_print(t->right, out);
			fprintf(out, ")");
			fprintf(out, " : %d\n", (int)btree_size(t));
		}
	}
}
