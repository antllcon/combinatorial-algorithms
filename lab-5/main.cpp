#include "class-genetic/genetic.h"
#include <bits/chrono.h>
#include <fstream>
#include <iostream>
#include <string>

int main()
{
	// Пример использования
	SetCoverProblem problem;
	problem.m = 5;
	problem.n = 4;
	problem.a
		= {{1, 0, 1, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 1, 0, 1}};
	problem.costs = {1.0, 2.0, 1.5, 1.2};

	GeneticSetCoverSolver solver(problem, 100, 1000, 0.1);
	Genome solution = solver.Solve();

	// Вывод решения
	std::cout << "Selected subsets: ";
	for (int j = 0; j < problem.n; ++j)
	{
		if (solution[j])
		{
			std::cout << j + 1 << " ";
		}
	}
	std::cout << std::endl;

	return 0;
	return 0;
}