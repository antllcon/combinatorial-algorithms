#ifndef ACO_H
#define ACO_H

#include "../class-ant/ant.h"
#include "../class-graph/graph.h"
#include <set>

class ACO
{
public:
	// Инициализация матрицы феромонов для графа
	// Установка начального значения феромона
	explicit ACO(Graph graph);
	~ACO() = default;

	// Получение самого выгодного пути,
	// найденного муравьями
	AntPath Optimize();

private:
	// Cоздание муравьев и установка
	// каждому начальной вершины
	void CreateAnts();

	// Исследование муравьев всех путей
	void ResearchAnts(AntPath& bestPath,
		Matrix& localPheromoneUpdate,
		int& counter,
		int countGraphVetices);

	// Глобальное обновление феромона,
	// получение на вход локальную соостовляющую обновления феромона
	void UpdateGlobalPheromone(const Matrix& localPheromone);

private:
	Graph m_graph;
	Matrix m_pheromone;
	std::vector<Ant> m_ants;
};

#endif // ACO_H