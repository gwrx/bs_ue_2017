#include "btree.h"

#include <assert.h>

struct btree
{
	btree_node* node;

	// store the size to return it in constant time
	size_t size;
};

struct btree_node
{
	btree_node* left;
	btree_node* right;
	int value;
};

btree* btree_create(void)
{
	btree* tree = malloc(sizeof(btree));

	if(tree == NULL)
	{
		return NULL;
	}

	tree->node = NULL;
	tree->size = 0;

	return tree;
}

size_t btree_size(const btree* t)
{
	assert(t);

	return t->size;
}

static btree_node* btree_node_create(int v)
{
	btree_node* node = malloc(sizeof(btree_node));
	assert(node);

	node->left = NULL;
	node->right = NULL;
	node->value = v;

	return node;
}

void btree_insert(btree* t, int d)
{
	assert(t);

	btree_node** walk_node = &t->node;

	while(*walk_node != NULL)
	{
		if((*walk_node)->value == d)
		{
			return;
		}

		if((*walk_node)->value < d)
		{
			walk_node = &(*walk_node)->right;
		}
		else
		{
			walk_node = &(*walk_node)->left;
		}
	}

	*walk_node = btree_node_create(d);
	++t->size;
}

static btree_node* btree_find_node(const btree* t, int d)
{
	btree_node* walk_node = t->node;

	while(walk_node != NULL)
	{
		if(walk_node->value == d)
		{
			return walk_node;
		}
		else if(walk_node->value < d)
		{
			walk_node = walk_node->right;
		}
		else
		{
			walk_node = walk_node->left;
		}
	}

	return NULL;
}

static btree_node* btree_find_node_parent_ptr(btree* t, int d, btree_node*** parent_node_ptr)
{
	btree_node* walk_node = t->node;
	btree_node** parent_ptr = &t->node;

	while(walk_node != NULL)
	{
		if(walk_node->value == d)
		{
			if(parent_node_ptr != NULL)
			{
				*parent_node_ptr = parent_ptr;
			}

			return walk_node;
		}

		if(walk_node->value < d)
		{
			parent_ptr = &walk_node->right;
			walk_node = walk_node->right;
		}
		else
		{
			parent_ptr = &walk_node->left;
			walk_node = walk_node->left;
		}
	}

	return NULL;
}

bool btree_contains(const btree* t, int d)
{
	assert(t);

	return btree_find_node(t, d) != NULL;
}

static int btree_node_maximum(const btree_node* node)
{
	while(node->right != NULL)
	{
		node = node->right;
	}

	return node->value;
}

int btree_maximum(const btree* t)
{
	assert(t);
	assert(t->node);

	return btree_node_maximum(t->node);
}

static int btree_node_minimum(const btree_node* node)
{
	while(node->left != NULL)
	{
		node = node->left;
	}

	return node->value;
}

int btree_minimum(const btree* t)
{
	assert(t);
	assert(t->node);

	return btree_node_minimum(t->node);
}

void btree_remove(btree* t, int d)
{
	assert(t);

	btree_node** parent_ptr;
	btree_node* node = btree_find_node_parent_ptr(t, d, &parent_ptr);

	// data was not found in the tree
	if(node == NULL)
	{
		return;
	}

	// difficult case: node with two children
	if(node->left != NULL && node->right != NULL)
	{
		// move predecessor data into this node and delete the predecessor (which is always an easier case)
		int predecessor = btree_node_maximum(node->left);
		btree_remove(t, predecessor);
		node->value = predecessor;
	}
	else
	{
		// if there is only one child just replace the node with the child (actually the pointer in the parent)
		if(node->left != NULL)
		{
			*parent_ptr = node->left;
		}
		else if(node->right != NULL)
		{
			*parent_ptr = node->right;
		}
		else // easy case: deleting a leaf
		{
			// just clear the pointer in the parent
			*parent_ptr = NULL;
		}

		free(node);
		--t->size;
	}
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

static void btree_destroy_node(btree_node* node)
{
	if(node == NULL)
	{
		return;
	}

	btree_destroy_node(node->left);
	btree_destroy_node(node->right);

	free(node);
}

void btree_destroy(btree* t)
{
	assert(t);

	btree_destroy_node(t->node);

	free(t);
}
