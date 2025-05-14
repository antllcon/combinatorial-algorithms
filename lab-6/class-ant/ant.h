#ifndef ANT_H
#define ANT_H

#include <vector>

using Route = std::vector<int>;

class Ant
{
public:
	Ant() = default;
	virtual ~Ant() = default;

	// Придумай методы для реализации
	void SetRoute(const Route& route);
	const Route& GetRoute() const;
	void AddCity(int city);

private:
	Route m_route;
};

#endif // ANT_H