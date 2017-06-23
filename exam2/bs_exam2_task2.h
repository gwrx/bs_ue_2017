#pragma once

#include <stdlib.h>
#include <pthread.h>

typedef struct graph_node {
	unsigned current_input;
	unsigned max;
	unsigned num_inputs_needed;
	struct graph_node* target;
	pthread_cond_t inputdata_avail;
	pthread_mutex_t mutex;
	pthread_t thread;
	/// YOUR DATA HERE
} graph_node;

// CURRENT:
// creates a new graph node, setting the number of inputs needed and the target
// a target of NULL indicates that this is a final node
// GOAL:
// also creates a thread to perform computations for this node
graph_node* create_node(unsigned num_inputs_needed, graph_node* target);

// CURRENT:
// destroys a given graph node
// GOAL:
// also ends the thread associated with the target
void destroy_node(graph_node* target);

// CURRENT:
// push the given input to a node in the graph and calls "perform_computation" directly
// GOAL:
// do NOT call "perform_computation" directly,
// but instead notify the thread associated with the node that new input data is available
// (use one condition variable per node)
void push_input(graph_node* node, unsigned input);

// uses the current data in "node" to update the state of the node
// if the number of remaining inputs needed is 0, pushes the result to the target
// GOAL:
// should only ever be executed by the thread associated with "node"
void perform_computation(graph_node* node);

// CURRENT:
// does nothing, as without threads it doesn't need to
// GOAL:
// cause the calling thread to wait until the thread associated with "node" is done
void wait_for_node(graph_node* node);
