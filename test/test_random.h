#include <string>
#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <cmath>

// Including this header file tests a random set of exchange rates constructed
// to have no arbitrage cycle

#define NUM_CURRENCIES 158
#define INPUT_FILE_PATH "test_random.txt"
#define OUTPUT_FILE_PATH "results_random.txt"
#define TOL .0000001
using namespace std;

const double MAX_LEN = (numeric_limits<double>::max() - 100)/2;
// Large enough to function as infinity, but doesn't cause overflows

/* Representation of a node:
	The predecessor node in the shortest path computed by Bellman-ford.
	The current shortest path length to the node, initialize to MAX_LEN.
	The ticker of the currency it represents.
	Tn array of edge lengths coming INTO the node:
		The source node for the edge is the position of that edge in the array
		in this representation. The node has an edge into itself with length 0,
		so that all from are alligned index wise with their source node. Since we used
		strict inequality in our relaxation step, this does not effect the algorithm.
		Missing data is given an edge length of MAX_LEN, again to ensure allignment
		and ensure that a non-existant edge will not be including in a shortest path.

	The currency graph is represented by an array of vertices.
	Implicitly, a node's id is it's position in the array.
*/
struct node{
	int predecessor;
	double cur_dist;
	char name[4];
	double from[NUM_CURRENCIES];
};

// Returns a true if the graph was initialized sucessfully based on the data in exchange_rates.txt
bool init_graph(node* graph);
