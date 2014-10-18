#include "graph.h"

/* Compares the current shortest path to node i to the shortest path to node j + the edge from j to i.
   Changes the current path length to this sum and change the predecessor to i to j if the latter
   is smaller.
*/
void relax(node* input, int to, int from){
	double comp = input[from].cur_dist + input[to].from[from];
	if(input[to].cur_dist > comp) {
		input[to].cur_dist = comp;
		input[to].predecessor = from;
	}
}

void relax_fordward(node* input){
	for(int i = 0; i < NUM_CURRENCIES; i++){
		for(int j = 0; j < i; j++){
			relax(input, i, j);
		}
	}
}

void relax_backward(node* input){
	for(int i = NUM_CURRENCIES-1; i >= 0; i--){
		for(int j = NUM_CURRENCIES-1; j > i; j--){
			relax(input, i, j);
		}
	}
}

// Relaxes all nodes (j) in the graph N-1 times (i).
// Computes the shortest path from node 0 to node j with at most i back steps on the ith iteration.
// Optimized to run in N*(N-1)/2 through yen's optimization
void bellman_ford(node* input){
	for(int i = 0; i < NUM_CURRENCIES/2; i++){
		relax_fordward(input);
		relax_backward(input);
	}
}	
