#ifndef GENETIC_H
#define GENETIC_H

#include <random>
#include <vector>

using Genome = std::vector<bool>;

struct SetCoverProblem
{
	int m;							  // Количество элементов
	int n;							  // Количество подмножеств
	std::vector<std::vector<bool>> a; // Матрица принадлежности (m x n)
	std::vector<double> costs;		  // Стоимости подмножеств
};

class GeneticSetCoverSolver
{
public:
	GeneticSetCoverSolver(
		const SetCoverProblem& problem, int pop_size, int max_gen, double mut_rate);

	void InitPopulation();

	// Вычисление стоимости решения
	double CalcCost(const Genome& solution);

	// Проверка, является ли решение допустимым (покрывает все элементы)
	bool IsFeasible(const Genome& solution);

	// Восстановление допустимости решения
	void RepairSolution(Genome& solution);

	// Удаление избыточных подмножеств
	void RemoveRedundantSubsets(Genome& solution, Genome& covered);

	// Вычисление значений приспособленности для всей популяции
	void EvalutePopulation();

	// Турнирный отбор
	int TournamentSelection(int tournament_size = 3);

	Genome Crossover(const Genome& parent1, const Genome& parent2);

	// Оператор мутации с переменной частотой
	void Mutate(Genome& solution);

	// Замена популяции (steady-state replacement)
	void ReplacePopulation(const Genome& offspring);

	// Основной метод решения
	Genome Solve();

private:
	SetCoverProblem problem;		// Параметры задачи
	int population_size;			// Размер популяции
	int max_generations;			// Число поколений
	double mutation_rate;			// Вероятность мутации
	std::vector<Genome> population; // Текущаяя популяция геномов
	std::vector<double> fitness_values; // Преспособленность решения

	std::mt19937 rng;
};

#endif // GENETIC_H