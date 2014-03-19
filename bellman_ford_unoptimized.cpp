#include "graph.h"

void relax(node* input, int i){
	double* from = input[i].from;

	for(int j = 0; j < NUM_CURRENCIES; j++){
		double comp = input[j].cur_dist + from[j];
		if(input[i].cur_dist > comp){
			input[i].cur_dist = comp;
			input[i].predecessor = j;
		}
	}
}

void bellman_ford(node* input){
	for(int i = 0; i < NUM_CURRENCIES-1; i++){
		for(int j = 0; j < NUM_CURRENCIES; j++){
			relax(input, j);
		}
	}
}
