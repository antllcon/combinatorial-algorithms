#include "class-graph/Graph.h"
#include <iostream>

int main()
{
	AdjacencyMatrix matrix = {{0, 1, 1}, {1, 0, 1}, {1, 1, 0}};
	Graph graph(matrix);
	VertexList list = graph.GetCycle(graph.GetMatrix());

	std::cout << "Цикл в графе: ";

	for (auto value : list)
	{
		std::cout << value << ' ';
	}

	std::cout << "\n";

	return EXIT_SUCCESS;
}