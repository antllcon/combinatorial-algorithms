#include "Lay.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>

Lay::Lay() = default;

void Lay::AddInitialCycle(const VertexList& cycle)
{
	faces.push_back(cycle);
	faces.emplace_back(cycle.rbegin(), cycle.rend());
}

FaceList Lay::GetFaces() const { return faces; }

FaceList Lay::FindSuitableFaces(const Segment& segment) const
{
	FaceList suitableFaces;

	for (const auto& face : faces)
	{
		std::unordered_set<int> faceVertices(face.begin(), face.end());
		bool allContactsInFace = true;

		for (int contact : segment.contacts)
		{
			if (!faceVertices.count(contact))
			{
				allContactsInFace = false;
				break;
			}
		}

		if (allContactsInFace)
		{
			suitableFaces.push_back(face);
		}
	}

	return suitableFaces;
}

bool Lay::AreContactsOnFace(const VertexList& contacts, const Face& face) const
{
	return std::all_of(contacts.begin(), contacts.end(), [&face](int contact) { return std::find(face.begin(), face.end(), contact) != face.end(); });
}

VertexList Lay::FindChainBetweenContacts(const Segment& segment, const Face& targetFace) const
{
	std::unordered_set<int> faceVertices(targetFace.begin(), targetFace.end());
	for (int contact : segment.contacts)
	{
		if (!faceVertices.count(contact))
		{
			std::cout << "Ошибка: контактная вершина не принадлежит грани.\n";
			return {};
		}
	}

	if (segment.contacts.empty())
	{
		std::cout << "Ошибка: сегмент не содержит контактных вершин.\n";
		return {};
	}

	std::unordered_set<int> visited;
	VertexList path;

	std::function<bool(int)> dfs = [&](int u) -> bool {
		path.push_back(u);
		visited.insert(u);

		if (segment.contacts.size() == 1)
		{
			if (path.size() > 1 && u == path.front())
			{
				std::cout << "Найден цикл возвращения в ту же контактную вершину: " << u << std::endl;
				return true;
			}
		}
		else
		{
			if (u != path.front() && std::find(segment.contacts.begin(), segment.contacts.end(), u) != segment.contacts.end())
			{
				std::cout << "Найден путь до другой контактной вершины: " << u << std::endl;
				return true;
			}
		}

		for (const auto& edge : segment.edges)
		{
			int v = (edge.first == u) ? edge.second : (edge.second == u ? edge.first : -1);
			if (v != -1 && (!visited.count(v) || (segment.contacts.size() == 1 && v == path.front())))
			{
				if (dfs(v))
				{
					return true;
				}
			}
		}
		path.pop_back();
		return false;
	};

	for (int contact : segment.contacts)
	{
		visited.clear();
		path.clear();
		if (dfs(contact))
		{
			return path;
		}
	}

	return {}; // Если путь не найден
}

void Lay::EmbedPath(const VertexList& path, const Face& targetFace)
{
	// Проверка, что концы цепи находятся в грани
	auto start_pos = std::find(targetFace.begin(), targetFace.end(), path.front());
	auto end_pos = std::find(targetFace.begin(), targetFace.end(), path.back());

	if (start_pos == targetFace.end() || end_pos == targetFace.end())
	{
		std::cerr << "Ошибка: концы цепи не найдены в грани\n";
		return;
	}

	// Определяем порядок обхода
	bool reverse_order = start_pos > end_pos;
	if (reverse_order)
		std::swap(start_pos, end_pos);

	// Создаем две новые грани
	Face newFace1, newFace2;

	// Первая грань: от start_pos до end_pos + цепь
	newFace1.insert(newFace1.end(), start_pos, end_pos + 1);
	newFace1.insert(newFace1.end(), path.begin() + 1, path.end() - 1);

	// Вторая грань: оставшаяся часть targetFace + цепь (учитываем порядок)
	newFace2.insert(newFace2.end(), end_pos, targetFace.end());
	newFace2.insert(newFace2.end(), targetFace.begin(), start_pos + 1);
	if (reverse_order)
		newFace2.insert(newFace2.end(), path.rbegin() + 1, path.rend() - 1);
	else
		newFace2.insert(newFace2.end(), path.begin() + 1, path.end() - 1);

	// Удаляем старую грань и добавляем новые
	auto face_it = std::find(faces.begin(), faces.end(), targetFace);
	if (face_it != faces.end())
		faces.erase(face_it); // Удаляем targetFace

	faces.push_back(newFace1);
	faces.push_back(newFace2);

	// Отладочный вывод
	std::cout << "Разбиение грани:\n";
	std::cout << "Новая грань 1: ";
	for (auto v : newFace1)
		std::cout << v + 1 << " ";
	std::cout << "\nНовая грань 2: ";
	for (auto v : newFace2)
		std::cout << v + 1 << " ";
	std::cout << std::endl;
}
