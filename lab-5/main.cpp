#include "class-genetic/genetic.h"
#include <fstream>
#include <iostream>
#include <string>

// Функция для чтения задачи из файла (формат OR-library)
SetCoverProblem read_problem_from_file(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Error opening file: " << filename << std::endl;
		exit(1);
	}

	SetCoverProblem problem;
	int m, n;
	file >> m >> n;

	problem.m = m;
	problem.n = n;
	problem.a.resize(m, std::vector<bool>(n, false));
	problem.costs.resize(n);

	// Читаем стоимости подмножеств
	for (int j = 0; j < n; ++j)
	{
		file >> problem.costs[j];
	}

	// Читаем матрицу принадлежности
	for (int i = 0; i < m; ++i)
	{
		int num_subsets;
		file >> num_subsets;
		for (int k = 0; k < num_subsets; ++k)
		{
			int j;
			file >> j;
			problem.a[i][j - 1] = true; // В файле индексы с 1
		}
	}

	file.close();
	return problem;
}

int main()
{
	SetCoverProblem problem;
	problem.m = 5;
	problem.n = 4;
	problem.a
		= {{1, 0, 1, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 1, 0, 1}};
	problem.costs = {1.0, 2.0, 3.0, 4.0};

	GeneticSetCoverSolver solver(problem, 100, 1000, 0.1);
	std::vector<bool> solution = solver.solve();

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
}