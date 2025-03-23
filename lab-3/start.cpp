#include "class-graph/graph.h"
#include <iostream>

int main()
{
	try
	{
		std::cout << "Создаем граф - 14 вершин\n";
		Graph graph(14, true);
		graph.AddEdge(1, 4);
		graph.AddEdge(1, 6);
		graph.AddEdge(2, 1);
		graph.AddEdge(2, 7);
		graph.AddEdge(3, 2);
		graph.AddEdge(3, 5);
		graph.AddEdge(3, 8);
		graph.AddEdge(4, 2);
		graph.AddEdge(4, 6);
		graph.AddEdge(4, 7);
		graph.AddEdge(5, 3);
		graph.AddEdge(5, 8);
		graph.AddEdge(6, 4);
		graph.AddEdge(6, 9);
		graph.AddEdge(8, 9);
		graph.AddEdge(8, 10);
		graph.AddEdge(9, 7);
		graph.AddEdge(10, 11);
		graph.AddEdge(11, 8);
		graph.AddEdge(12, 14);
		graph.AddEdge(12, 13);
		graph.AddEdge(13, 14);

		std::cout << graph << std::endl;

		Components strongComp = graph.GetStrongComponentsKosaraju();

		std::cout << "Его компоненты сильной связности:" << std::endl;
		std::cout << strongComp << std::endl;
	}
	catch (const std::exception& errorMessage)
	{
		std::cerr << errorMessage.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}