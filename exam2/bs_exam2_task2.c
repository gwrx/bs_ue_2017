#include "bs_exam2_task2.h"
#include <stdio.h>
#include <stdlib.h>


graph_node* create_node(unsigned num_inputs_needed, graph_node* target) {
	graph_node* ret = (graph_node*)malloc(sizeof(graph_node));
	ret->max = 0;
	ret->num_inputs_needed = num_inputs_needed;
	ret->target = target;

	pthread_mutex_t mp = PTHREAD_MUTEX_INITIALIZER;

	if (pthread_mutex_init(&mp, NULL) != 0) {
		printf("mutex error");
		exit(EXIT_FAILURE);
	}
	
	return ret;
}

void destroy_node(graph_node* target) {
	free(target);
}

void push_input(graph_node* node, unsigned input) {
	node->current_input = input;
	pthread_cond_signal(&node->inputdata_avail);
	pthread_create(&node->thread, NULL, (void*)perform_computation, (graph_node*)&node);
}

void perform_computation(graph_node* node) {
	if(node->current_input > node->max) {
		pthread_cond_signal(&node->inputdata_avail);
		pthread_mutex_lock(&node->mutex);
		node->max = node->current_input;
	}
	node->num_inputs_needed--;
	if(node->num_inputs_needed == 0 && node->target != NULL) {
		push_input(node->target, node->max);
		pthread_mutex_unlock(&node->mutex);
	}
}

void wait_for_node(graph_node* node) {

	pthread_join(node->thread, NULL);
	// in sequential program we don't need to do anything here
}

