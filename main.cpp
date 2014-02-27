#include "graph.h"

/* Compares the current shortest path to node i to the shortest path to node j + the edge from j to i.
   Changes the current path length to this sum and change the predecessor to i to j if the latter
   is smaller.
*/
void relax(node* input, int i){
	int last_step = input[i].predecessor;
	double* from_from = input[i].from;

	for(int j = 0; j < NUM_CURRENCIES; j++){
		double comp = input[j].cur_dist + from_from[j];
		if(input[i].cur_dist > comp) {
			input[i].cur_dist = comp;
			input[i].predecessor = j;
		}
	}
}

// Relaxes all nodes (j) in the graph N-1 times (i).
// Computes the shortest path from node 0 to node j with at most i back steps on the ith iteration.
// I believe this can be optimized to run in N*(N-1)/2
void bellman_ford(node* input){
	node* temp;

	for(int i = 0; i < NUM_CURRENCIES-1; i++)
		for(int j = 0; j < NUM_CURRENCIES; j++)
			relax(input, j);
}	

/* Backtracks along the predecessor chain to find the index of where a negative cycle beings.
   Uses a local bitmap to determine whether we have visited a node in this iteration, thus
   determining we are in a cycle. Uses a global bitmap to determine whether we have already
   found this cycle in our search for negative cycles.
   Returns -1 if we have already visited this cycle, or we reached a node with no predecessor.
*/ 
int find_cycle_index(node* input, bool* visited_global, int node){
	bool visited_local[NUM_CURRENCIES] = { false };
	
	while(!visited_local[node]){
		if(visited_global[node])
			return -1;
		
		visited_global[node] = true;
		visited_local[node] = true;

		node = input[node].predecessor;

		// Not in a negative cycle
		if(node == -1){
			cout << "Not in an abitrage loop" << endl;
			return -1;
		}	
	}

	return node;
}

/* If we have found a node index associated with a negative cycle that we haven't visited,
   backtrack along the predecessor chain, computing product of the exchange rates, and  outputting
   the currency pairs. Due to the fact we're following a predecessor chain, it prints the sequence
   in reverse, although in practice this shouldn't matter.
*/
void print_negative_cycle(node* input, bool* visited_global, int first_node, ofstream & outfile){
	int cycle_start = find_cycle_index(input, visited_global, first_node);

	if(cycle_start != -1){
		int cur_node = cycle_start, last_node = cycle_start;

		cout << endl << "New arbitrage cycle detected: " << endl;
		if(OUTPUT_FILE_PATH != "results.txt")
			outfile << "Computed using test data, not valid" << endl;
		
		double arbitrage_return = 0;

		do{
			last_node = cur_node;
			cur_node = input[cur_node].predecessor;	
			arbitrage_return += input[last_node].from[cur_node];

			cout << input[cur_node].name << input[last_node].name
			     << " " << exp(-input[last_node].from[cur_node]) << endl;

			outfile << input[cur_node].name << input[last_node].name << endl;

		} while(cur_node != cycle_start);

		outfile << "------" << endl;

		cout << "For a total return of " << (exp(-arbitrage_return)-1)*100 << "%" 
		     << endl << endl; 
	}
}

/* Checks if adding an edge to the shortest path reduces it's length.
   If it does, there is a negative cycle associated with that node, and print that cycle.
   Tracks the node's we've visited already in our search, so that we don't print a cycle twice.
   Checks vs. a tolerance to hopefully ignore any errors accumulated through floating point addition.
*/
void find_negative_cycles(node* input){
	bool visited[NUM_CURRENCIES] = { false };
	bool found = false;

	ofstream outfile;
	outfile.open(OUTPUT_FILE_PATH);

	for(int i = 0; i < NUM_CURRENCIES; i++){
		for(int j = 0; j < NUM_CURRENCIES; j++){
			if(input[i].cur_dist > input[j].cur_dist + input[i].from[j] + TOL){
				found = true;
				print_negative_cycle(input, visited, i, outfile);	
			}
		}
	}	

	if(!found){
		cout << "No arbitrage cycle found." << endl;		
		outfile << "No arbitrage cycle found." << endl;
	}

	outfile.close();
}


int main(){
	node currency_graph[NUM_CURRENCIES];

	if(init_graph(currency_graph)){
		bellman_ford(currency_graph);
		find_negative_cycles(currency_graph);
	}
	else{
		cout << "Error initializing graph" << endl;
		return -1;
	}
	return 0;
}

