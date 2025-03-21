#include "class-graph/graph.h"
#include <iostream>

int main()
{
	try
	{
		AdjacencyMatrix matrix = {
			{ 0, 1, 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 0, 0, 1, 0, 1 },
			{ 0, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 1, 0, 0, 1 },
			{ 0, 0, 1, 0, 0, 0, 0, 0, 0 }
		};
	}
	catch (const std::exception& errorMessage)
	{
		std::cerr << errorMessage.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}