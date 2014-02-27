#include "graph.h"

// Contains functions pertaining to the initialization of the exchange rate graph

bool comp_name(node* graph, string input, int index){
	if(graph[index].name[0] != input[0])
		return true;
	if(graph[index].name[1] != input[1])
		return true;
	if(graph[index].name[2] != input[2])
		return true;

	return false;
}

void init_name(node* graph, string input, int index){
	graph[index].name[0] = input[0];
	graph[index].name[1] = input[1];
	graph[index].name[2] = input[2];
	graph[index].name[3] = input[3];
}

void init_cur_dist(node* graph){
	// Start at node 0
	graph[0].cur_dist = 0;

	for(int i = 1; i < NUM_CURRENCIES; i++)
		graph[i].cur_dist = MAX_LEN;
}

void init_edge_length(node* graph, int from, int to, double len){
	graph[to].from[from] = len;
}

void init_last_step(node* graph){		
	// Initialize all paths to start at a non-existant node
	for(int i = 0; i < NUM_CURRENCIES; i++)
		graph[i].predecessor = -1;
}

/* Reads data in from the input file and initialize the graph.
   Relies on valid input data to sucessfully initialize, and won't give an error on most invalid
   cases. Input data should be of the form:
	NAME NAME RATE
   For an arbitrary ordering on the nodes, the data should come as all edges from node i to j,
   in order of first i and then j. The output of get_exchange_rates_async.py is alphabetically ordered
*/
bool init_graph(node* input){
	ifstream infile;
	infile.open(INPUT_FILE_PATH);

	if(!infile.is_open()){
		cout << "Failed to open file" << endl;
		return false;
	}

	string from, to ,rate;

	for(int i = 0; i < NUM_CURRENCIES; i++){
		for(int j = 0; j <NUM_CURRENCIES; j++){
			if(infile.eof()){
				cout << "Invalid input file: incomplete currency pair list" << endl;
				return false;
			}

			infile >> from >> to >> rate;

			if(i == 0){
				init_name(input,to, j);
			}

			if(comp_name(input, to, j) || comp_name(input, from, i)){
				cout << "Input order error: invalid currency pair ordering at line " 
				     << i*NUM_CURRENCIES + j << endl;
				return false;
			}
				
			if(i == j)
				init_edge_length(input,i,j,0);
			else if(rate != "default" && rate != "N/A")
				init_edge_length(input,i,j,-log(atof(rate.c_str())));
			else{
				init_edge_length(input,i,j,MAX_LEN);
				cout << "Missing data for " << from << " to " << to << endl;
			}
		}
	}

	init_cur_dist(input);
	init_last_step(input);

	infile.close();

	return true;
}
