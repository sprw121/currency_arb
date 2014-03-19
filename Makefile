all:
	g++ graph.cpp bellman_ford_optimized.cpp main.cpp -o main

unoptimized:
	g++ graph.cpp bellman_ford_unoptimized.cpp main.cpp -o main
