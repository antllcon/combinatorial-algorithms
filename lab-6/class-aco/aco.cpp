#include "aco.h"

ACO::ACO(Graph graph, int numAnts, int numCities, int numIterations)
	: m_visibility(AdjacencyMatrix(numCities))
	, m_pheromones(AdjacencyMatrix(numCities))
	, m_deltaPheromones(AdjacencyMatrix(numCities))
	, m_probs(AdjacencyMatrix(numCities))
{
	// Создание графа видимости
	m_visibility = graph.CreateVisibilityGraph();

	// Инициализация феромонов
	for (int i = 0; i < numCities; i++)
	{
		for (int j = 0; j < numCities; j++)
		{
			m_pheromones.SetValue(i, j, constants::pheromone);
		}
	}

	// Инициализация лучшего маршрута
	m_bestRoute = std::vector<int>(numCities, std::numeric_limits<int>::max());

	// Создание муравьев
	for (int k = 0; k < numAnts; ++k)
	{
		m_ants.insert(Ant());
	}
}

// int ACO::ValidRoute(Ant& ant, int iteration)
// {
// 	for (int i = 0; i < GetCitiesCount() - 1; i++)
// 	{
// 		int cityi = ROUTES[antk][i];
// 		int cityj = ROUTES[antk][i + 1];
// 		if (cityi < 0 || cityj < 0)
// 		{
// 			return -1;
// 		}
// 		if (!exists(cityi, cityj))
// 		{
// 			return -2;
// 		}
// 		for (int j = 0; j < i - 1; j++)
// 		{
// 			if (ROUTES[antk][i] == ROUTES[antk][j])
// 			{
// 				return -3;
// 			}
// 		}
// 	}

// 	if (!exists(INITIALCITY, ROUTES[antk][NUMBEROFCITIES - 1]))
// 	{
// 		return -4;
// 	}

// 	return 0;
// }

void ACO::Optimize(int numIterations)
{
	for (int iterations = 1; iterations <= numIterations; ++iterations)
	{
		for (int k = 0; k < GetAntsCount(); ++k)
		{

			// 1. Формируем маршрут

			m_ants[k].ClearRoute();
			m_ants[k].AddCity(startCity);

			Route route;
			do
			{
				route = FindRoute(k);
			} while (!ValidRoute(route));

			// 2) Сохраняем маршрут внутрь объекта Ant
			m_ants[k].SetRoute(route);

			// 3) Обновляем лушчий маршрут
			if (LengthRoute(route) < m_bestRoute)
			{
				m_bestRoute = route;
			}
		}

		cout << endl << "updating PHEROMONES . . .";
		updatePHEROMONES();
		cout << " done!" << endl << endl;
		printPHEROMONES();

		for (int i = 0; i < NUMBEROFANTS; i++)
		{
			for (int j = 0; j < NUMBEROFCITIES; j++)
			{
				ROUTES[i][j] = -1;
			}
		}

		cout << endl << "ITERATION " << iterations << " HAS ENDED!" << endl << endl;
	}
}

Route ACO::FindRoute(int antk)
{
	Route route;
	route.reserve(GetCitiesCount());

	// 1) Начинаем из 0 города
	int current = 0;
	route.push_back(current);

	// 2) Пометка посещённых городов
	std::vector<bool> visited(GetCitiesCount(), false);
	visited[current] = true;

	// 3) Каждый следующий город
	for (int step = 1; step < GetCitiesCount(); ++step)
	{
		// Собираем пары (вероятность, город) для всех доступных непосещённых соседей
		std::vector<std::pair<double, int>> probs;
		probs.reserve(GetCitiesCount());

		double sum = 0.0;
		auto neigh = m_visibility.Neighbors(current);
		for (int nxt : neigh)
		{
			if (visited[nxt])
				continue;

			double tau
				= std::pow(m_pheromones.Weight(current, nxt), constants::alpha);
			double eta = std::pow(m_visibility.Weight(current, nxt), constants::beta);
			double w = tau * eta;
			probs.emplace_back(w, nxt);
			sum += w;
		}

		// Если тупик — возвращаем неполный маршрут
		if (probs.empty())
		{
			return route;
		}

		// Заполняем m_probs - ?
		for (auto& p : probs)
		{
			m_probs.SetValue(current, p.second, p.first);
		}

		// Случайный выбор на основе относительных весов
		double xi = randoms->Uniforme() * sum;
		double acc = 0.0;
		int chosen = probs.back().second; // на случай погрешностей
		for (auto& p : probs)
		{
			acc += p.first;
			if (acc >= xi)
			{
				chosen = p.second;
				break;
			}
		}

		// Добавляем в маршрут
		route.push_back(chosen);
		visited[chosen] = true;
		current = chosen;
	}

	return route;
}

int ACO::GetCitiesCount() const { return m_visibility.GetMatrix().size(); }

int ACO::GetAntsCount() const { return static_cast<int>(m_ants.size()); }
