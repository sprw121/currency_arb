This project finds currency arbitrage cycles, defined as a chain of currency transactions that yield a profit. Included are a script to get exchange rates from Yahoo API, a C++ program that output arbitrage cycles, if any exist, various tests, and a script that verifies the results vs. another query to Yahoo. Arbitrage cycles are found using an implementation of the Bellman-Ford algorithm in main.cpp

The C++ portion was written with performance considerations in mind, not the most flexible implementation.

Further optimization could be done.

See below for a brief description of each file:

currencies.txt: 
	Defines a list of currencies to be used. Any change to this list should result in a corresponding change to NUM_CURRENCIES inside graph.h and test_random.h.

get_exchange_rates_async.py:
	A python script that takes the list in currencies.txt, formats queries for all pairs, queries yahoo, then prints the parsed results into exchange_rates.txt. Yahoo API allows a maximum of 250 pairs per request, so we accomplish this with a number of asynchronous requests. May not warn user of bad results.

graph.h:
	Defines the format of the graph, and declares various constants used in graph.cpp and main.cpp

test_random.h:
	Defines the format of the graph, and different input and output constants than graph.h so that we read in a random graph constructed to have no arbitrage cycles, and output to a different results file.

generate_random_graph.py:
	Generated a random currency graph with all exchange rates in the range (.1,1). This ensures no arbitrage cycles. To run a test, change the header files in graph.cpp and main.cpp to test_random.h.

test_known.h:
	Defines the format of the graph, and different input and output constants than graph.h so that we read in a  graph with a known arbitrage cycle, and output to a different results file.

test_known.txt:
	A 5 currencies example with an easily verifiable arbitrage cycle.

graph.cpp:
	Contains functions pertainent to initializing the graph. To run a test, change the header file to either test_random.h or test_known.h and recompile.

main.cpp:
	Finds and arbitrage cycles using the Bellman-Ford algorithm. Also contains functions that backtrack along the shortest paths chain to print a negative cycle. To run a test, change the header file to either test_random.h or test_known.h and recompile.

verify_results.py:
	Takes the result output from main.cpp in results.txt, and formats and makes a new request to the Yahoo API. Verifies the results vs. this new data. Any discrepancies between the results of main.cpp and verify_results.py are most likely due to a change in exchange rates.

run_script.sh:
	Compiles main.cpp, gets the exchange rates from Yahoo, runs main, and verifies results.

/misc/ :
	Contains:
		 exchange_rates.txt as of 8:46PM, Feb 26, 2013,
		 results.txt an arbitrage cycle of the above exchange rates, using main.cpp.
		 test_random.txt, a graph I randomly generated using generate_random_graph.py
		 results_random.txt, the results of main.cpp on test_random.txt
		 known_results.txt, the results of main.cpp on test_known.txt
