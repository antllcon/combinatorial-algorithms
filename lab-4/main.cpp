#include "class-graph/Graph.h"
#include <iostream>

int main()
{
	std::string fileName = "../test/data.txt";

	try
	{
		FileToGraphAdapter adapter;
		Graph graph(adapter.ConvertEdgeListToMatrix(fileName));

		if (graph.IsPlanarGraph())
		{
			std::cout << "[Yes]" << std::endl;
		}
		else
		{
			std::cout << "[No]" << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}