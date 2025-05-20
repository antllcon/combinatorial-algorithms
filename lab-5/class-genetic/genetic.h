#ifndef GENETIC_H
#define GENETIC_H

#include <random>
#include <vector>

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

	// Инициализация случайной популяции
	void initialize_population();

	// Вычисление стоимости решения
	double calculate_cost(const std::vector<bool>& solution);

	// Проверка, является ли решение допустимым (покрывает все элементы)
	bool is_feasible(const std::vector<bool>& solution);

	// Восстановление допустимости решения
	void repair_solution(std::vector<bool>& solution);

	// Удаление избыточных подмножеств
	void remove_redundant_subsets(
		std::vector<bool>& solution, std::vector<bool>& covered);

	// Вычисление значений приспособленности для всей популяции
	void evaluate_population();

	// Турнирный отбор
	int tournament_selection(int tournament_size = 3);

	std::vector<bool> crossover(
		const std::vector<bool>& parent1, const std::vector<bool>& parent2);

	// Оператор мутации с переменной частотой
	void mutate(std::vector<bool>& solution);

	// Замена популяции (steady-state replacement)
	void replace_population(const std::vector<bool>& offspring);

	// Основной метод решения
	std::vector<bool> solve();

private:
	SetCoverProblem problem;
	int population_size;
	int max_generations;
	double mutation_rate;
	std::vector<std::vector<bool>> population;
	std::vector<double> fitness_values;
	std::mt19937 rng;
};

#endif // GENETIC_H