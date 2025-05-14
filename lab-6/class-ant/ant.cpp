#include "ant.h"

void Ant::SetRoute(const Route& route) { m_route = route; }

const Route& Ant::GetRoute() const { return m_route; }

void Ant::AddCity(int city) { m_route.push_back(city); }
