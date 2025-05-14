#include "class-aco/aco.h"
#include "class-graph/graph.h"
#include <climits>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>

// beta = 0 - суб оптимальное решение
// alpla = 0 - случайный поиск + суб оптимальное решение
// Q - оценка предполагаемого лучшего маршрута

int main()
{
	// const std::string file = "../test/k-five.txt";
	// FileToGraphAdapter adapter;
	// Graph graph(adapter.ConvertEdgeListToMatrix(file));

	AdjacencyMatrix matrix = {{0, 1, 2, 3, 4, 5},
		{1, 0, 5, 9, 13, 17},
		{2, 5, 0, 4, 1, 2},
		{3, 9, 4, 0, 20, 10},
		{4, 13, 1, 20, 0, 15},
		{5, 17, 2, 10, 15, 0}};

	int numCities = matrix.size();
	int numAnts = matrix.size();
	int numIterations = matrix.size();

	Graph graph(matrix);

	ACO* ants = new ACO(graph, numAnts, numCities, numIterations);

	// ants->init();

	// ants->connectCITIES(0, 1);
	// ants->connectCITIES(0, 2);
	// ants->connectCITIES(0, 3);
	// ants->connectCITIES(0, 7);
	// ants->connectCITIES(1, 3);
	// ants->connectCITIES(1, 5);
	// ants->connectCITIES(1, 7);
	// ants->connectCITIES(2, 4);
	// ants->connectCITIES(2, 5);
	// ants->connectCITIES(2, 6);
	// ants->connectCITIES(4, 3);
	// ants->connectCITIES(4, 5);
	// ants->connectCITIES(4, 7);
	// ants->connectCITIES(6, 7);
	/* ants -> connectCITIES(8, 2);
	ants -> connectCITIES(8, 6);
	ants -> connectCITIES(8, 7); */

	// ants->setCITYPOSITION(0, 1, 1);
	// ants->setCITYPOSITION(1, 10, 10);
	// ants->setCITYPOSITION(2, 20, 10);
	// ants->setCITYPOSITION(3, 10, 30);
	// ants->setCITYPOSITION(4, 15, 5);
	// ants->setCITYPOSITION(5, 10, 1);
	// ants->setCITYPOSITION(6, 20, 20);
	// ants->setCITYPOSITION(7, 20, 30);
	// ants -> setCITYPOSITION(8, 26, 20);

	// ants->printGRAPH();

	// ants->printPHEROMONES();

	// Запуск
	// ants->optimize(constants::iterations);

	// ants->printRESULTS();

	return 0;
}
