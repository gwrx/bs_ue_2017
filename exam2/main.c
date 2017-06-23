#include "bs_exam2_task2.h"

#include <stdio.h>

int main() {

	// build a graph of this shape:
	// | | |  | |  |
	// 0 1 2  3 4  5
	//  \|/   \ / /
	//   6     7 /
	//    \   / /
	//     \ | /
	//      \|/
	//       8

	graph_node* nodes[9];

	// start at the end (third layer - target NULL)
	nodes[8] = create_node(3, NULL);

	// second layer
	nodes[7] = create_node(2, nodes[8]);
	nodes[6] = create_node(3, nodes[8]);

	// first layer
	nodes[5] = create_node(1, nodes[8]);

	nodes[4] = create_node(1, nodes[7]);
	nodes[3] = create_node(1, nodes[7]);

	nodes[2] = create_node(1, nodes[6]);
	nodes[1] = create_node(1, nodes[6]);
	nodes[0] = create_node(1, nodes[6]);

	// push inputs into the first layer
	for(int i = 0; i <= 5; ++i) {
		push_input(nodes[i], i + 10);
	}

	// wait until the computation is done
	wait_for_node(nodes[8]);

	// output result
	printf("Result: %d\n", nodes[8]->max);

	// clean up
	for(int i = 0; i < 9; ++i) {
		destroy_node(nodes[i]);
	}
}
