#include "aco.h"
#include "../constants.h"

ACO::ACO(Graph graph)
	: m_graph(graph)
{
	auto count = m_graph.GetSize();
	Matrix matrix(count, constants::pheromone);
	m_pheromone = std::move(matrix);
}

void ACO::CreateAnts()
{
	// Количество муравьев должно быть равно
	// количеству вершин графа
	auto count = m_graph.GetSize();
	m_ants.resize(count);

	for (size_t i = 0; i != count; ++i)
	{
		m_ants[i] = Ant(i);
	}
}

void ACO::UpdateGlobalPheromone(const Matrix& localPheromone)
{
	int size = localPheromone.GetSize();

	for (int from = 0; from != size; ++from)
	{
		for (int to = 0; to != size; ++to)
		{
			m_pheromone[from][to] = constants::weathering * m_pheromone[from][to]
				+ localPheromone[from][to];

			if (m_pheromone[from][to] < 0.01 and from != to)
			{
				m_pheromone[from][to] = 0.01;
			}
		}
	}
}

AntPath ACO::Optimize()
{
	std::cout << "Начало поиска пути" << std::endl;
	if (m_graph.IsEmpty())
	{
		return {};
	}

	int count = m_graph.GetSize();
	// int numIterations = count;
	int numIterations = 100;
	int counter = 0;

	AntPath bestPath;
	bestPath.distance = std::numeric_limits<double>::max();

	while (counter++ != numIterations)
	{
		// std::cout << "Итерация №" << counter << std::endl;
		Matrix localPheromoneUpdate(count, constants::pheromone);

		CreateAnts();
		// std::cout << "Муравьи созданы" << std::endl;

		ResearchAnts(bestPath, localPheromoneUpdate, counter, count);
		// std::cout << "Муравьи все исследовали" << std::endl;

		UpdateGlobalPheromone(localPheromoneUpdate);
		// std::cout << "Муравьи оставили ферамоны" << std::endl;
	}

	return bestPath;
}

void ACO::ResearchAnts(AntPath& bestPath,
	Matrix& localPheromoneUpdate,
	int& counter,
	int countGraphVetices)
{
	for (auto& ant : m_ants)
	{
		while (ant.GetMove())
		{
			ant.MakeChoice(m_graph, m_pheromone);
		}

		AntPath path = ant.GetBestPath();
		int countVertices = path.vertices.size();
		// std::cout << "Муравей нашел свой путь" << std::endl;

		if (countVertices != countGraphVetices + 1)
		{
			// std::cout << "Путь недостаточно большой" << std::endl;
			continue;
		}

		// Проверка на корректность индексов
		if (countVertices < 2)
		{
			// std::cout << "Ошибка: путь слишком короткий!" << std::endl;
			continue;
		}

		if (bestPath.distance > path.distance)
		{
			bestPath = std::move(path);
			counter = 0;
		}
		// std::cout << "Лучший путь обновлен" << std::endl;

		// for (auto i : bestPath.vertices)
		// {
		// 	std::cout << i << " ";
		// }
		// std::cout << std::endl;

		for (int v = 0; v != countVertices - 1; ++v)
		{
			int from = bestPath.vertices[v];
			int to = bestPath.vertices[v + 1];

			if (from != to)
			{
				localPheromoneUpdate[from][to] += constants::q / path.distance;
			}
		}
	}
}