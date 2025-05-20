#ifndef ANT_H
#define ANT_H

#include "../class-graph/graph.h"
#include <random>
#include <vector>

using Route = std::vector<int>;
using VertexList = std::vector<int>;
using ProbList = std::vector<double>;

struct AntPath
{
	Route vertices;
	double distance = 0;
};

class Ant
{
public:
	Ant();
	explicit Ant(int start);

	bool GetMove() const;
	AntPath GetBestPath() const;

	// Получение случайного шанса в заданном диапазоне
	double GetRandomChance(double success, double total);

	// Получение возможных для посещения вершин
	// от текущего положения
	VertexList GetNeighbors(const Graph& graph);

	// Выбор следующей вершины
	void MakeChoice(const Graph& graph, const Matrix& matrix);

private:
	ProbList CalculateProb(const VertexList& neighbors,
		int current,
		const Graph& graph,
		const Matrix& matrix);

private:
	AntPath m_path;
	Route m_checked;
	int m_start = 0;
	int m_current = 0;
	bool m_move = true;
	std::mt19937 m_mt;
};

#endif // ANT_H