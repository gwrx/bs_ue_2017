#include "btree.h"
#include <assert.h>

	struct btree {
	btree_node *node;
	btree *left;
	btree *right;
	size_t size;
	int depth;
};


struct btree_node {
	int value;
};


btree* btree_create() { // empty parameterlist is undefined - use void as parameter to ensure no argument is given
	btree *tree = (btree*)malloc(sizeof(btree));
	assert(tree != NULL);
	// don't use assert here - asserts can be removed by compiler
	// return something useful for the user of this module and let him handle the error
	tree->node = NULL;
	tree->left = tree->right = NULL;
	tree->size = 0;
	tree->depth = 0;

	return tree;
}


//own function to remove the link to the subtree
//check if value below is NULL cut the link to the freeed node
static void btree_remove_link(btree* t) {
	// use assert here - if this function is called with null-pointer the user messed up
	if ((t->left != NULL) && (t->left->node == NULL)) {
		free(t->left);
		t->left = NULL;
	}
	if ((t->right != NULL) && (t->right->node == NULL)) {
		free(t->right);
		t->right = NULL;
	}
}


/// Destroys and deallocates all memory for the given tree 't'
void btree_destroy(btree* t) {
	// use assert here - if this function is called with null-pointer the user messed up
	if (t != NULL) {
		if (t->left != NULL) {
			btree_destroy(t->left);
			btree_remove_link(t);
		}
		if (t->right != NULL) {
			btree_destroy(t->right);
			btree_remove_link(t);
		}

		//free node
		if ((t->left == NULL) && (t->right == NULL)) {
			free(t->node);
			t->node = NULL;
		}
		
		//free root
		if (t->depth == 0) {
			free(t);
		}
	}
}


/// Inserts the given number 'd' into tree 't'
/// if the number is already in 't', no changes are made
void btree_insert(btree* t, int d) {
	if (!btree_contains(t, d)) {
		t->size++;
		if (t->node == NULL) {
			t->node = (btree_node*)malloc(sizeof(btree_node));
			assert(t->node != NULL);
			t->node->value = d;
			t->left = t->right = NULL;
		} else {
			if (t->node->value > d) {
				if (t->left == NULL) {
					t->left = btree_create();
					t->left->depth = t->depth + 1;	
				}
				btree_insert(t->left, d);
			} else {
				if (t->right == NULL) {
					t->right = btree_create();
					t->right->depth = t->depth + 1;
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
			btree_remove_link(t);
		} else if (t->node->value < d) {
			btree_remove(t->right, d);
			btree_remove_link(t);
		} else {
			//found node
			//base case: node is a leaf in the tree
			if ((t->left == NULL) && (t->right == NULL)) {
				free(t->node);	
				t->node = NULL;
			//second case: node has one child, left or right
			} else if ((t->left == NULL) || (t->right == NULL)) {
				//left subtree is set
				if (t->left != NULL) {
					free(t->node);
					free(t);
					t = t->left;
					t->depth--;
				//right subtree is set
				} else {
					free(t->node);
					free(t);
					t = t->right;
					t->depth--;
				}
			//third case, tree has two subtrees
			//get max of left subtree and replace searched value with the found max
			//then recursive call to remove the max from left subtree
			} else {
				//replace node, with max of left subtree
				int max_node = btree_maximum(t->left);
				t->node->value = max_node;
				btree_remove(t->left, max_node);
				btree_remove_link(t);
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
	//depth 0 is the root of the tree
	if (t->depth == 0) {
		fprintf(out, "(");
	}
	if (t->node == NULL) {
		fprintf(out, "NIL");
	} else {
		if (t->left != NULL) {
			fprintf(out, "(");
			btree_print(t->left, out);
			fprintf(out, "), ");
		}

		fprintf(out, "%d", t->node->value);

		if (t->right != NULL) {
			fprintf(out, ", (");
			btree_print(t->right, out);
			fprintf(out, ")");
		}
	}

	//depth of the tree 0, root case
	if (t->depth == 0) {
		fprintf(out, ") : %d\n", (int)btree_size(t));
	}
}

