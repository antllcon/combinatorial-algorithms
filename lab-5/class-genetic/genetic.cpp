#include "genetic.h"
#include "chrono"
#include <algorithm>
#include <iostream>
#include <limits>
#include <random>

using namespace std::chrono;

GeneticSetCoverSolver::GeneticSetCoverSolver(
	const SetCoverProblem& problem, int pop_size, int max_gen, double mut_rate)
	: problem(problem)
	, population_size(pop_size)
	, max_generations(max_gen)
	, mutation_rate(mut_rate)
{
	rng.seed(system_clock::now().time_since_epoch().count());
}

// Инициализация случайной популяции
void GeneticSetCoverSolver::initialize_population()
{
	population.resize(population_size);
	std::uniform_int_distribution<int> dist(0, 1);

	for (int i = 0; i < population_size; ++i)
	{
		population[i].resize(problem.n);
		for (int j = 0; j < problem.n; ++j)
		{
			population[i][j] = dist(rng);
		}
		repair_solution(population[i]);
	}
}

// Вычисление стоимости решения
double GeneticSetCoverSolver::calculate_cost(const std::vector<bool>& solution)
{
	double cost = 0.0;
	for (int j = 0; j < problem.n; ++j)
	{
		if (solution[j])
		{
			cost += problem.costs[j];
		}
	}
	return cost;
}

// Проверка, является ли решение допустимым (покрывает все элементы)
bool GeneticSetCoverSolver::is_feasible(const std::vector<bool>& solution)
{
	for (int i = 0; i < problem.m; ++i)
	{
		bool covered = false;
		for (int j = 0; j < problem.n; ++j)
		{
			if (solution[j] && problem.a[i][j])
			{
				covered = true;
				break;
			}
		}
		if (!covered) return false;
	}
	return true;
}

// Восстановление допустимости решения
void GeneticSetCoverSolver::repair_solution(std::vector<bool>& solution)
{
	std::vector<bool> covered(problem.m, false);
	std::vector<int> selected_subsets;

	// Собираем уже выбранные подмножества
	for (int j = 0; j < problem.n; ++j)
	{
		if (solution[j])
		{
			selected_subsets.push_back(j);
			for (int i = 0; i < problem.m; ++i)
			{
				if (problem.a[i][j]) covered[i] = true;
			}
		}
	}

	// Проверяем, все ли элементы покрыты
	bool all_covered = true;
	for (bool cov : covered)
	{
		if (!cov)
		{
			all_covered = false;
			break;
		}
	}

	if (all_covered)
	{
		// Удаляем избыточные подмножества
		remove_redundant_subsets(solution, covered);
		return;
	}

	// Добавляем подмножества, пока не покроем все элементы
	while (!all_covered)
	{
		// Находим элемент, который не покрыт
		int uncovered_element = -1;
		for (int i = 0; i < problem.m; ++i)
		{
			if (!covered[i])
			{
				uncovered_element = i;
				break;
			}
		}

		// Находим подмножество с наилучшим соотношением стоимость/покрытие
		int best_subset = -1;
		double best_ratio = std::numeric_limits<double>::max();

		for (int j = 0; j < problem.n; ++j)
		{
			if (!solution[j] && problem.a[uncovered_element][j])
			{
				int coverage = 0;
				for (int i = 0; i < problem.m; ++i)
				{
					if (!covered[i] && problem.a[i][j])
					{
						coverage++;
					}
				}
				double ratio = problem.costs[j] / coverage;
				if (ratio < best_ratio)
				{
					best_ratio = ratio;
					best_subset = j;
				}
			}
		}

		// Добавляем лучшее подмножество
		if (best_subset != -1)
		{
			solution[best_subset] = true;
			selected_subsets.push_back(best_subset);
			for (int i = 0; i < problem.m; ++i)
			{
				if (problem.a[i][best_subset]) covered[i] = true;
			}
		}

		// Проверяем, все ли покрыто
		all_covered = true;
		for (bool cov : covered)
		{
			if (!cov)
			{
				all_covered = false;
				break;
			}
		}
	}

	// Удаляем избыточные подмножества
	remove_redundant_subsets(solution, covered);
}

// Удаление избыточных подмножеств
void GeneticSetCoverSolver::remove_redundant_subsets(
	std::vector<bool>& solution, std::vector<bool>& covered)
{
	// Сортируем подмножества в порядке убывания стоимости
	std::vector<std::pair<double, int>> subsets;
	for (int j = 0; j < problem.n; ++j)
	{
		if (solution[j])
		{
			subsets.emplace_back(problem.costs[j], j);
		}
	}
	sort(subsets.rbegin(), subsets.rend());

	// Проверяем каждое подмножество на избыточность
	for (auto& [cost, j] : subsets)
	{
		bool redundant = true;
		for (int i = 0; i < problem.m; ++i)
		{
			if (problem.a[i][j] && covered[i])
			{
				// Проверяем, есть ли другие подмножества, покрывающие этот элемент
				int count = 0;
				for (int k = 0; k < problem.n; ++k)
				{
					if (solution[k] && problem.a[i][k])
					{
						count++;
					}
				}
				if (count <= 1)
				{
					redundant = false;
					break;
				}
			}
		}

		if (redundant)
		{
			solution[j] = false;
			for (int i = 0; i < problem.m; ++i)
			{
				if (problem.a[i][j])
				{
					covered[i] = false;
					// Восстанавливаем покрытие другими подмножествами
					for (int k = 0; k < problem.n; ++k)
					{
						if (solution[k] && problem.a[i][k])
						{
							covered[i] = true;
							break;
						}
					}
				}
			}
		}
	}
}

// Вычисление значений приспособленности для всей популяции
void GeneticSetCoverSolver::evaluate_population()
{
	fitness_values.resize(population_size);
	for (int i = 0; i < population_size; ++i)
	{
		fitness_values[i] = calculate_cost(population[i]);
	}
}

// Турнирный отбор
int GeneticSetCoverSolver::tournament_selection(int tournament_size)
{
	std::uniform_int_distribution<int> dist(0, population_size - 1);
	int best = dist(rng);
	double best_fitness = fitness_values[best];

	for (int i = 1; i < tournament_size; ++i)
	{
		int candidate = dist(rng);
		if (fitness_values[candidate] < best_fitness)
		{
			best = candidate;
			best_fitness = fitness_values[candidate];
		}
	}

	return best;
}

// Улучшенный оператор кроссовера (из статьи)
std::vector<bool> GeneticSetCoverSolver::crossover(
	const std::vector<bool>& parent1, const std::vector<bool>& parent2)
{
	std::vector<bool> child(problem.n);
	std::vector<double> p0(problem.n, 0.0), p1(problem.n, 0.0);

	// Вычисляем частоты генов в популяции
	for (int j = 0; j < problem.n; ++j)
	{
		int count0 = 0, count1 = 0;
		for (const auto& individual : population)
		{
			if (individual[j])
				count1++;
			else
				count0++;
		}
		p0[j] = static_cast<double>(count0) / population_size;
		p1[j] = static_cast<double>(count1) / population_size;
	}

	double f1 = calculate_cost(parent1);
	double f2 = calculate_cost(parent2);

	for (int j = 0; j < problem.n; ++j)
	{
		if (parent1[j] == parent2[j])
		{
			child[j] = parent1[j];
		}
		else
		{
			if (parent1[j] == 0)
			{
				child[j] = (f1 * p0[j] >= f2 * p1[j]) ? parent1[j] : parent2[j];
			}
			else
			{
				child[j] = (f1 * p1[j] >= f2 * p0[j]) ? parent1[j] : parent2[j];
			}
		}
	}

	return child;
}

// Оператор мутации с переменной частотой
void GeneticSetCoverSolver::mutate(std::vector<bool>& solution)
{
	std::vector<double> p0(problem.n, 0.0), p1(problem.n, 0.0);
	std::vector<double> entropy(problem.n, 0.0);

	// Вычисляем частоты и энтропию для каждого гена
	for (int j = 0; j < problem.n; ++j)
	{
		int count0 = 0, count1 = 0;
		for (const auto& individual : population)
		{
			if (individual[j])
				count1++;
			else
				count0++;
		}
		p0[j] = static_cast<double>(count0) / population_size;
		p1[j] = static_cast<double>(count1) / population_size;

		// Вычисляем энтропию
		if (p0[j] > 0 && p1[j] > 0)
		{
			entropy[j] = -p0[j] * log(p0[j]) - p1[j] * log(p1[j]);
		}
		else
		{
			entropy[j] = 0.0;
		}
	}

	// Применяем мутацию
	for (int j = 0; j < problem.n; ++j)
	{
		double mutation_prob = (1.0 / problem.n) * (1.0 - entropy[j]);
		std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

		if (prob_dist(rng) < mutation_prob)
		{
			if (solution[j] && p1[j] > p0[j])
			{
				solution[j] = false;
			}
			else if (!solution[j] && p0[j] > p1[j])
			{
				solution[j] = true;
			}
		}
	}
}

// Замена популяции (steady-state replacement)
void GeneticSetCoverSolver::replace_population(const std::vector<bool>& offspring)
{
	// Находим среднюю приспособленность
	double avg_fitness = 0.0;
	for (double f : fitness_values)
	{
		avg_fitness += f;
	}
	avg_fitness /= population_size;

	// Находим кандидатов для замены (с приспособленностью выше средней)
	std::vector<int> candidates;
	for (int i = 0; i < population_size; ++i)
	{
		if (fitness_values[i] > avg_fitness)
		{
			candidates.push_back(i);
		}
	}

	if (!candidates.empty())
	{
		std::uniform_int_distribution<int> dist(0, candidates.size() - 1);
		int replace_index = candidates[dist(rng)];
		population[replace_index] = offspring;
		fitness_values[replace_index] = calculate_cost(offspring);
	}
}

// Основной метод решения
std::vector<bool> GeneticSetCoverSolver::solve()
{
	auto start_time = high_resolution_clock::now();

	// Инициализация популяции
	initialize_population();
	evaluate_population();

	int generation = 0;
	int no_improvement = 0;
	double best_fitness = *min_element(fitness_values.begin(), fitness_values.end());
	std::vector<bool> best_solution
		= population[min_element(fitness_values.begin(), fitness_values.end())
			- fitness_values.begin()];

	while (generation < max_generations && no_improvement < 50)
	{
		// Выбор родителей
		int parent1 = tournament_selection();
		int parent2 = tournament_selection();

		// Кроссовер
		std::vector<bool> offspring
			= crossover(population[parent1], population[parent2]);

		// Мутация
		mutate(offspring);

		// Восстановление допустимости
		repair_solution(offspring);

		// Проверяем, есть ли уже такая особь в популяции
		bool duplicate = false;
		for (const auto& individual : population)
		{
			if (individual == offspring)
			{
				duplicate = true;
				break;
			}
		}

		if (!duplicate)
		{
			// Замена в популяции
			replace_population(offspring);

			// Обновляем лучший результат
			double current_best
				= *min_element(fitness_values.begin(), fitness_values.end());
			if (current_best < best_fitness)
			{
				best_fitness = current_best;
				best_solution = population[min_element(fitness_values.begin(),
											   fitness_values.end())
					- fitness_values.begin()];
				no_improvement = 0;
			}
			else
			{
				no_improvement++;
			}
		}

		generation++;
	}

	auto end_time = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end_time - start_time);

	std::cout << "Algorithm finished in " << generation << " generations ("
			  << duration.count() << " ms)" << std::endl;
	std::cout << "Best solution cost: " << best_fitness << std::endl;
	std::cout << "Solution is feasible: "
			  << (is_feasible(best_solution) ? "YES" : "NO") << std::endl;

	return best_solution;
}
