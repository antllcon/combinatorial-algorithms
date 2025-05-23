#include "class-chrono/Timer.h"
#include "class-genetic/Genetic.h"
#include "class-greedy/Greedy.h"
#include <bits/chrono.h>
#include <fstream>
#include <iostream>
#include <string>

SetCoverProblem GenerateSetCover(int m, int n, double density)
{
	SetCoverProblem problem;
	problem.m = m;
	problem.n = n;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	std::uniform_real_distribution<double> cost_dist(0.5, 5.0);

	problem.a.resize(m, std::vector<bool>(n, false));
	for (int i = 0; i < m; ++i)
	{
		std::cout << i + 1 << " - [ ";
		for (int j = 0; j < n; ++j)
		{
			if (dist(gen) < density)
			{
				std::cout << 1 << " ";
				problem.a[i][j] = true;
			}
			else
			{
				std::cout << 0 << " ";
			}
		}
		std::cout << "]" << std::endl;
	}

	problem.costs.resize(n);
	for (int j = 0; j < n; ++j)
	{
		problem.costs[j] = cost_dist(gen);
	}

	return problem;
}

int main()
{
	constexpr int n = 50;
	constexpr int m = 30;
	constexpr double density = 0.3;
	SetCoverProblem problem = GenerateSetCover(n, m, density);
	SetCoverProblem problemNoCash = problem;
	problemNoCash.costs = std::vector<double>(n, 1.0);

	GeneticSetCoverSolver solver(problem, m, 1000, 0.02);
	Genome solutionGenetic = solver.Solve();

	std::cout << "Лучший ген (деньги): ";
	GeneticSetCoverSolver::Print(n, solutionGenetic);

	GeneticSetCoverSolver solverNoCash(problemNoCash, m, 1000, 0.02);
	Genome solutionGenNoCash = solverNoCash.Solve();

	std::cout << "Лучший ген (нет денег): ";
	GeneticSetCoverSolver::Print(n, solutionGenNoCash);

	Genome solutionGreendy = GreedySetCover(problem);
	std::cout << "Лучший набор: ";
	GeneticSetCoverSolver::Print(n, solutionGenetic);
	return 0;
}