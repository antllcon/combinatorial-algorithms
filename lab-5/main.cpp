#include "class-genetic/genetic.h"
#include <bits/chrono.h>
#include <fstream>
#include <iostream>
#include <string>

int main()
{
	// Параметры задачи
	const int m = 10; // число элементов
	const int n = 15; // число подмножеств

	// Инициализируем генератор случайных чисел
	std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<double> cost_dist(1.0, 10.0);
	std::bernoulli_distribution pick_dist(0.3);

	// Формируем задачу
	SetCoverProblem problem;
	problem.m = m;
	problem.n = n;
	problem.costs.resize(n);
	problem.a.assign(m, std::vector<bool>(n, false));

	// Случайные стоимости
	for (int j = 0; j < n; ++j)
	{
		problem.costs[j] = cost_dist(rng);
	}

	// Случайная матрица, затем добиваем гарантированное покрытие
	for (int j = 0; j < n; ++j)
	{
		for (int i = 0; i < m; ++i)
		{
			// с вероятностью 30% включаем i-й элемент в j-е подмножество
			if (pick_dist(rng))
			{
				problem.a[i][j] = true;
			}
		}
	}
	// Гарантируем, что каждый элемент покрыт хотя бы одним подмножеством
	for (int i = 0; i < m; ++i)
	{
		bool covered = false;
		for (int j = 0; j < n; ++j)
		{
			if (problem.a[i][j])
			{
				covered = true;
				break;
			}
		}
		if (!covered)
		{
			// если не покрыт — добавляем его в случайное подмножество
			std::uniform_int_distribution<int> subset_dist(0, n - 1);
			int j = subset_dist(rng);
			problem.a[i][j] = true;
		}
	}

	// Параметры генетического алгоритма
	int population_size = 100;
	int max_generations = 1000;
	double mutation_rate = 0.1;

	// Запускаем решатель
	GeneticSetCoverSolver solver(
		problem, population_size, max_generations, mutation_rate);
	std::vector<bool> solution = solver.solve();

	// Вывод результата
	double total_cost = 0.0;
	std::cout << "Selected subsets: ";
	for (int j = 0; j < n; ++j)
	{
		if (solution[j])
		{
			std::cout << (j + 1) << " ";
			total_cost += problem.costs[j];
		}
	}
	std::cout << "\nTotal cost = " << total_cost << std::endl;

	// 1) Проверим, что total_cost совпадает с суммой стоимостей выбранных подмножеств
	double check_cost = 0.0;
	for (int j = 0; j < problem.n; ++j)
	{
		if (solution[j]) check_cost += problem.costs[j];
	}
	std::cout << "Check cost sum = " << check_cost << " (matches? "
			  << (fabs(check_cost - total_cost) < 1e-6 ? "YES" : "NO") << ")\n";

	// 2) Выведем покрытие каждого элемента
	std::vector<bool> covered(problem.m, false);
	for (int j = 0; j < problem.n; ++j)
	{
		if (solution[j])
		{
			for (int i = 0; i < problem.m; ++i)
			{
				if (problem.a[i][j]) covered[i] = true;
			}
		}
	}
	std::cout << "Element coverage:\n";
	for (int i = 0; i < problem.m; ++i)
	{
		std::cout << " Element " << i + 1 << ": "
				  << (covered[i] ? "covered" : "NOT covered") << "\n";
	}

	// 3) Если все элементы покрыты, feasible должен быть YES
	bool feasible = solver.is_feasible(solution);
	std::cout << "Final feasibility check: " << (feasible ? "YES" : "NO")
			  << std::endl;

	return 0;
}