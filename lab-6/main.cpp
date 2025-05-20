#include "class-aco/aco.h"
#include "class-graph/graph.h"
#include <climits>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>

using AdjMatrix = std::vector<std::vector<double>>;

int main()
{
	const std::string file = "../../test/graph.txt";
	FileToGraphAdapter adapter;
	Graph graph(adapter.ConvertEdgeListToMatrix(file));

	// graph.PrintGraph();

	// AdjMatrix matrix = {{0, 1, 2, 3, 4, 5},
	// 	{1, 0, 5, 9, 13, 17},
	// 	{2, 5, 0, 4, 1, 2},
	// 	{3, 9, 4, 0, 20, 10},
	// 	{4, 13, 1, 20, 0, 15},
	// 	{5, 17, 2, 10, 15, 0}};

	// Matrix matr(matrix);
	// Graph graph(matr);
	ACO aco(graph);

	AntPath result = aco.Optimize();

	std::cout << "Лучший путь: " << result.distance << std::endl;
	for (int v : result.vertices)
	{
		std::cout << v << " ";
	}
	std::cout << "\n";

	return 0;
}
