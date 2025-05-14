#ifndef ACO_H
#define ACO_H

#include "../class-ant/ant.h"
#include "../class-graph/graph.h"
#include "Randoms.cpp"
#include <set>

namespace constants
{
inline constexpr double alpha = 0.5;
inline constexpr double beta = 0.8;
inline constexpr double q = 10;
inline constexpr double pheromonesEvaporation = 0.2;
inline constexpr int randMaxPheromone = 2;
inline constexpr double pheromone = 0.1;
}; // namespace constants

class ACO
{
public:
	ACO(Graph visibility, int numAnts, int numCities, int numIterations);
	virtual ~ACO();

	// Пока по методам не знаю, в процессе пойму
	void Optimize(int numIterations);

	int GetCitiesCount() const;
	int GetAntsCount() const;

private:
	int ValidRoute(Ant& ant);
	Route FindRoute(int antk);

private:
	// Видимость каждого города
	Graph m_visibility;

	// Феромоны между городами
	Graph m_pheromones;

	// Феромоны (на каждой итерации)
	Graph m_deltaPheromones;

	// Вероятности между городами
	Graph m_probs;

	// Маршруты пройденые каждым муравьем
	std::vector<Route> m_routes;

	// Лушчий маршрут
	Route m_bestRoute;

	// Муравьи
	std::vector<Ant> m_ants;

	Randoms* randoms;
};

#endif // ACO_H