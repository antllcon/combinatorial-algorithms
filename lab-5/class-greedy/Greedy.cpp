#ifndef GREEDY_H
#define GREEDY_H

#include "../class-chrono/Timer.h"
#include "../class-genetic/Genetic.h"
#include <limits>
#include <vector>

Genome GreedySetCover(const SetCoverProblem& problem)
{
	Timer timer("Жадный алгоритм");
	const int m = problem.m;
	const int n = problem.n;
	const auto& a = problem.a;
	const auto& costs = problem.costs;

	Genome solution(n, false);			   // Инициализируем решение (все множества не выбраны)
	std::vector<bool> covered(m, false);   // Отслеживаем покрытые элементы
	std::vector<int> uncoveredCount(n, 0); // Количество непокрытых элементов для каждого множества

	// Инициализируем счетчики непокрытых элементов
	for (int j = 0; j < n; ++j)
	{
		for (int i = 0; i < m; ++i)
		{
			if (a[i][j])
			{
				uncoveredCount[j]++;
			}
		}
	}

	int coveredCount = 0; // Количество уже покрытых элементов

	while (coveredCount < m)
	{
		double minCostPerElem = std::numeric_limits<double>::max();
		int bestSet = -1;

		// Ищем множество с минимальной стоимостью на один непокрытый элемент
		for (int j = 0; j < n; ++j)
		{
			if (solution[j] || uncoveredCount[j] == 0) continue;

			double costPerElem = costs[j] / uncoveredCount[j];
			if (costPerElem < minCostPerElem)
			{
				minCostPerElem = costPerElem;
				bestSet = j;
			}
		}

		if (bestSet == -1) break; // Нет подходящих множеств

		// Добавляем лучшее множество в решение
		solution[bestSet] = true;

		// Обновляем покрытые элементы и счетчики
		for (int i = 0; i < m; ++i)
		{
			if (!covered[i] && a[i][bestSet])
			{
				covered[i] = true;
				coveredCount++;

				// Уменьшаем счетчики для всех множеств, содержащих этот элемент
				for (int j = 0; j < n; ++j)
				{
					if (a[i][j])
					{
						uncoveredCount[j]--;
					}
				}
			}
		}
	}

	return solution;
}

#endif // GREEDY_H