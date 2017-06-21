#include "btree.h"
#include <assert.h>


struct btree {
	btree_node* node;
	size_t size;
	int depth;
};

struct btree_node {
	int value;
	btree* left;
	btree* right;
};


btree* btree_create(void) { // empty parameterlist is undefined - use void as parameter to ensure no argument is given

	btree* tree = malloc(sizeof(btree));
	if (tree == NULL) {
		printf("creation of btree failed");
	}

	tree->node = NULL;
	tree->size = 0;
	tree->depth = 0;

	return tree;
}




/// Destroys and deallocates all memory for the given tree 't'
void btree_destroy(btree* t) {
	// use assert here - if this function is called with null-pointer the user messed up
	if (t != NULL) {
		if (t->node->left != NULL) {
			btree_destroy(t->node->left);
			btree_remove_link(t);
		}
		if (t->node->right != NULL) {
			btree_destroy(t->node->right);
			btree_remove_link(t);
		}

		//free node
		if ((t->node->left == NULL) && (t->node->right == NULL)) {
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
			t->node = malloc(sizeof(btree_node));
			assert(t->node != NULL);
			t->node->value = d;
			t->node->left = t->node->right = NULL;
		} else {
			if (t->node->value > d) {
				if (t->node->left == NULL) {
					t->node->left = btree_create();
					t->depth++;	
				}
				btree_insert(t->node->left, d);
			} else {
				if (t->node->right == NULL) {
					t->node->right = btree_create();
					t->depth++;	
				}
				btree_insert(t->node->right, d);
			}
		}
	}
}


/// Removes the given number 'd' from tree 't'
void btree_remove(btree* t, int d) {
	if (btree_contains(t, d)) {
			t->size--;
		if (t->node->value > d) {
			btree_remove(t->node->left, d);
			btree_remove_link(t);
		} else if (t->node->value < d) {
			btree_remove(t->node->right, d);
			btree_remove_link(t);
		} else {
			//found node
			//base case: node is a leaf in the tree
			if ((t->node->left == NULL) && (t->node->right == NULL)) {
				free(t->node);	
				t->node = NULL;
			//second case: node has one child, left or right
			} else if ((t->node->left == NULL) || (t->node->right == NULL)) {
				//left subtree is set
				if (t->node->left != NULL) {
					free(t->node->left);
					free(t);
					t = t->node->left;
					t->depth--;
				//right subtree is set
				} else {
					free(t->node->right);
					free(t);
					t = t->node->right;
					t->depth--;
				}
			//third case, tree has two subtrees
			//get max of left subtree and replace searched value with the found max
			//then recursive call to remove the max from left subtree
			} else {
				//replace node, with max of left subtree
				int max_node = btree_maximum(t->node->left);
				t->node->value = max_node;
				btree_remove(t->node->left, max_node);
				btree_remove_link(t);
			}
		}
	}
}


/// Returns the smallest number in tree 't'
int btree_minimum(const btree* t) {
	if (t->node->left == NULL) {
		return t->node->value;
	} else {
		return btree_minimum(t->node->left);
	}
}


/// Returns the largest number in tree 't'
int btree_maximum(const btree* t) {
	if (t->node->right == NULL) {
		return t->node->value;
	} else {
		return btree_maximum(t->node->right);
	}
}


/// Returns true if the given tree 't' contains 'd', false otherwise
bool btree_contains(const btree* t, int d) {
	if (t->node != NULL) {
		if (d < t->node->value) {
			if (t->node->left != NULL) {
				return btree_contains(t->node->left, d);
			}
		} else if (d > t->node->value) {
			if (t->node->right != NULL) {
				return btree_contains(t->node->right, d);
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

static void btree_print_node(btree_node* node, FILE* out)
{
	fputs("(", out);

	if(node->left)
	{
		btree_print_node(node->left, out);
		fputs(", ", out);
	}

	fprintf(out, "%d", node->value);

	if(node->right)
	{
		fputs(", ", out);
		btree_print_node(node->right, out);
	}

	fputs(")", out);
}

/// Prints the given btree 't' to the supplied output stream 'out'
/// output format: ([LEFT], VAL, [RIGHT]) : [SIZE]
/// example empty: ( NIL ) : 0
/// example 3,4,7 in a balanced tree: ((3), 4, (7)) : 3
void btree_print(const btree* t, FILE* out)
{
	assert(t);

	if(t->node == NULL)
	{
		fputs("( NIL )", out);
	}
	else
	{
		btree_print_node(t->node, out);
	}

	fprintf(out, " : %zd\n", t->size);
}

//own function to remove the link to the subtree
//check if value below is NULL cut the link to the freeed node
void btree_remove_link(btree* t) {
	// use assert here - if this function is called with null-pointer the user messed up
	if ((t->node->left != NULL) && (t->node->left->node == NULL)) {
		free(t->node->left);
		t->node->left = NULL;
	}
	if ((t->node->right != NULL) && (t->node->right->node == NULL)) {
		free(t->node->right);
		t->node->right = NULL;
	}
}
