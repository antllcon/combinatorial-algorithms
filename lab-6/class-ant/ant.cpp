#include "ant.h"
#include "../constants.h"
#include <algorithm>
Ant::Ant()
	: m_start(0)
	, m_current(0)
	, m_move(true)
	, m_mt(std::random_device{}())
{
}

Ant::Ant(int start)
	: m_start(start)
	, m_current(start)
	, m_move(true)
	, m_mt(std::random_device{}())
{
}

bool Ant::GetMove() const
{
	return m_move;
}

AntPath Ant::GetBestPath() const
{
	return m_path;
}

double Ant::GetRandomChance(double success, double total)
{
	std::uniform_real_distribution<double> dist(success, total);
	return dist(m_mt);
}

VertexList Ant::GetNeighbors(const Graph& graph)
{
	VertexList vertices;
	int size = graph.GetSize();

	for (size_t to = 0; to != size; ++to)
	{
		bool exist = graph[m_current][to] != 0;
		bool free
			= std::find(m_checked.begin(), m_checked.end(), to) == m_checked.end();

		if (exist && free)
		{
			vertices.push_back(to);
		}
	}
	return vertices;
}

void Ant::MakeChoice(const Graph& graph, const Matrix& matrix)
{
	if (m_path.vertices.empty())
	{
		m_path.vertices.push_back(m_current);
		m_checked.push_back(m_current);
	}

	VertexList neighbors = GetNeighbors(graph);

	if (neighbors.empty())
	{
		m_move = false;
		if (graph[m_current][m_start] != 0)
		{
			m_path.vertices.push_back(m_start);
			m_path.distance += graph[m_current][m_start];
		}
		return;
	}

	// Получаем вероятности перехода
	ProbList prob = CalculateProb(neighbors, m_current, graph, matrix);

	// Формируем кумулятивную сумму для рулетки
	ProbList choosingProb(prob.size());
	choosingProb[0] = prob[0];
	for (size_t i = 1; i < prob.size(); ++i)
	{
		choosingProb[i] = choosingProb[i - 1] + prob[i];
	}

	// Выбор вершины для перехода в диапазоне
	int nextVertex = neighbors.back();
	double choose = GetRandomChance(0.0, 1.0);
	for (int n = 0; n != neighbors.size(); ++n)
	{
		if (choose <= choosingProb[n])
		{
			nextVertex = neighbors[n];
			break;
		}
	}

	// Переход к выбранной вершине
	m_path.vertices.push_back(nextVertex);
	m_path.distance += graph[m_current][nextVertex];
	m_checked.push_back(nextVertex);
	m_current = nextVertex;
}

ProbList Ant::CalculateProb(const VertexList& neighbors,
	int current,
	const Graph& graph,
	const Matrix& matrix)
{
	ProbList wish;
	double summaryWish = 0.0;

	for (auto v : neighbors)
	{
		double t = matrix[current][v];
		double w = graph[current][v];
		double n = 1.0 / w;
		double value = std::pow(t, constants::alpha) * std::pow(n, constants::beta);
		wish.push_back(value);
		summaryWish += value;
	}

	ProbList prob;
	for (auto val : wish)
	{
		prob.push_back(val / summaryWish);
	}
	return prob;
}