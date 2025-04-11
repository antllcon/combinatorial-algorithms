#include "Graph.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>

Graph::Graph(const AdjacencyMatrix& matrix)
	: matrix(matrix)
	, vertexCount(matrix.size())
{
	AssertIsMatrixSize(vertexCount);
	AssertIsMatrixSquare(matrix);
}

AdjacencyMatrix Graph::GetMatrix() const { return matrix; }

int Graph::GetVertexCount() const { return vertexCount; }

VertexList Graph::GetCycle(const AdjacencyMatrix& currentMatrix)
{
	AssertIsMatrixSquare(currentMatrix);
	StateList list(vertexCount, color::white);
	VertexList parent(vertexCount, -1);
	VertexList cycle;

	for (int i = 0; i < vertexCount; ++i)
	{
		if (list[i] != color::white)
		{
			continue;
		}
		if (DeepthFirstSearchVisit(i, list, parent, cycle, currentMatrix))
		{
			std::reverse(cycle.begin(), cycle.end());
			return cycle;
		}
	}

	return {};
}

bool Graph::IsPlanarGraph()
{
	// Инициализация
	AdjacencyMatrix workGraph = matrix;
	Lay lay;

	// Получение первого цикла
	VertexList cycle = GetCycle(workGraph);

	std::cout << "Цикл:\n";
	for (auto val : cycle)
	{
		std::cout << val + 1 << ' ';
	}
	std::cout << std::endl << std::endl;

	// Добавление цикла в укладку
	lay.AddInitialCycle(cycle);

	std::cout << "Текущие грани укладки:\n";
	for (auto line : lay.GetFaces())
	{
		std::cout << '[';
		for (auto val : line)
		{
			std::cout << val + 1 << ' ';
		}
		std::cout << ']' << std::endl;
	}
	std::cout << std::endl;

	// Перемещаем первый цикл в список
	VertexList planarGraph = cycle;
	RemoveCycleEdges(workGraph, cycle);

	std::cout << "Матрица смежности после удаления цикла:\n";
	for (auto line : workGraph)
	{
		std::cout << '[';
		for (auto val : line)
		{
			std::cout << val << ' ';
		}
		std::cout << ']' << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Список вершин (уложенных из цикла):\n";
	for (auto val : planarGraph)
	{
		std::cout << val + 1 << ' ';
	}
	std::cout << std::endl << std::endl;
	while (true)
	{
		std::cout << " ============== \n\n";

		// Поиск контактных вершин
		VertexList contacts = FindContactVertices(workGraph, planarGraph);

		// TODO: вынести проверку контактных вершин
		if (contacts.size() == 0)
		{
			return true;
		}

		std::cout << "Найденые контактные вершины:\n";
		for (auto val : contacts)
			std::cout << val + 1 << ' ';
		std::cout << std::endl << std::endl;

		// Отладка workGraph

		std::cout << "Отладка workGraph:\n";
		for (auto line : workGraph)
		{
			std::cout << '[';
			for (auto val : line)
			{
				std::cout << val << ' ';
			}
			std::cout << ']' << std::endl;
		}
		std::cout << std::endl;

		// Разбиенение на сегменты
		SegmentList segments = FindSegments(workGraph, contacts);

		if (segments.size() == 0)
		{
			return true;
		}

		std::cout << "Нашли сегменты:\n";
		for (auto line : segments)
		{
			for (auto val : line.vertices)
			{
				std::cout << val + 1 << ' ';
			}
			std::cout << "контактные вершины: ";
			for (auto val : line.contacts)
			{
				std::cout << val + 1 << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;

		// Выбрать сегмент S с минимальным ∣Γ(S)∣ (наименьшим выбором граней)
		Segment selectedSegment = FindSegmentWithMinFaces(segments, lay);

		std::cout << "Минимальное кол-во граней у сегмента с вершинами: \n";
		for (auto val : selectedSegment.vertices)
		{
			std::cout << val + 1 << ' ';
		}
		std::cout << std::endl << std::endl;

		if (selectedSegment.contacts.empty())
		{
			std::cout << "Сегмент без контактов - граф непланарен\n";
			return false;
		}

		// Найти множество граней Γ(S), куда его можно уложить
		FaceList suitableFaces = lay.FindSuitableFaces(selectedSegment);

		if (suitableFaces.empty())
		{
			std::cout << "Некуда укладывать (нет Γ(S))\n";
			return false;
		}

		std::cout << "Множество граней для укладки сегмента:\n";
		for (auto line : suitableFaces)
		{
			std::cout << '[';
			for (auto val : line)
			{
				std::cout << val + 1 << ' ';
			}
			std::cout << ']' << std::endl;
		}
		std::cout << std::endl;

		Face targetFace = suitableFaces.front();
		std::cout << "Выбрана грань для укладки -> ";
		for (auto val : targetFace)
		{
			std::cout << val + 1 << ' ';
		}
		std::cout << std::endl << std::endl;

		if (selectedSegment.vertices.empty())
		{
			std::cout << "Ошибка: пустой сегмент\n";
			return false;
		}

		if (targetFace.empty())
		{
			std::cout << "Ошибка: пустая грань\n";
			return false;
		}

		// Уложить P в грань F
		VertexList path = lay.FindChainBetweenContacts(selectedSegment, targetFace);

		for (auto val : path)
		{
			std::cout << val + 1 << " ";
		}
		std::cout << std::endl;

		if (path.empty())
		{
			std::cout << "Ошибка: нет путя для укладки\n";
			return false;
		}
		lay.EmbedPath(path, targetFace);
		RemoveChainEdges(workGraph, path);

		std::cout << "Матрица смежности после удаления цикла:\n";
		for (auto line : workGraph)
		{
			for (auto val : line)
			{
				std::cout << val << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;

		AddPath(planarGraph, path);

		std::cout << "Список вершин (уложенных из цикла):\n";
		for (auto val : planarGraph)
		{
			std::cout << val + 1 << ' ';
		}

		std::cout << "\nМножество всех граней:\n";
		for (auto line : lay.GetFaces())
		{
			std::cout << '[';
			for (auto val : line)
			{
				std::cout << val + 1 << ' ';
			}
			std::cout << ']' << std::endl;
		}
		std::cout << std::endl;
	}

	return true;
}

void Graph::AddPath(VertexList& planarGraph, const VertexList& path)
{
	std::unordered_set<int> exists(planarGraph.begin(), planarGraph.end());

	for (auto vertex : path)
	{
		if (exists.insert(vertex).second)
		{
			planarGraph.push_back(vertex);
		}
	}
}
VertexList Graph::FindContactVertices(const AdjacencyMatrix& workGraph, const VertexList& planarGraph)
{
	VertexList contactVertices;
	std::unordered_set<int> uniqueContacts;

	for (int vertex : planarGraph)
	{
		for (size_t j = 0; j < workGraph.size(); ++j)
		{
			if (workGraph[vertex][j] || workGraph[j][vertex])
			{
				if (uniqueContacts.insert(vertex).second)
				{
					contactVertices.push_back(vertex);
				}
				break;
			}
		}
	}

	return contactVertices;
}

SegmentList Graph::FindSegments(const AdjacencyMatrix& workGraph, const VertexList& contacts)
{
	SegmentList segments;
	std::unordered_set<int> contactSet(contacts.begin(), contacts.end());
	std::vector<bool> visited(workGraph.size(), false);

	// 1. Обработка одиночных рёбер между контактами
	for (size_t i = 0; i < contacts.size(); ++i)
	{
		for (size_t j = i + 1; j < contacts.size(); ++j)
		{
			int u = contacts[i], v = contacts[j];
			if (workGraph[u][v])
			{
				Segment segment;
				segment.contacts = {u, v};
				segment.edges = {{u, v}};
				segments.push_back(segment);
				visited[u] = visited[v] = true;
			}
		}
	}

	// 2. Поиск всех связных компонентов
	for (int contact : contacts)
	{
		if (!visited[contact])
		{
			Segment segment;
			std::stack<int> stack;
			stack.push(contact);
			visited[contact] = true;
			segment.contacts.push_back(contact);

			while (!stack.empty())
			{
				int current = stack.top();
				stack.pop();

				for (int neighbor = 0; neighbor < workGraph.size(); ++neighbor)
				{
					if (workGraph[current][neighbor] && !visited[neighbor])
					{
						stack.push(neighbor);
						visited[neighbor] = true;
						segment.edges.emplace_back(current, neighbor);

						if (contactSet.count(neighbor))
						{
							segment.contacts.push_back(neighbor);
						}
						else
						{
							segment.vertices.push_back(neighbor);
						}
					}
				}
			}

			// Добавляем только если сегмент содержит хотя бы 1 контактную вершину
			if (!segment.contacts.empty())
			{
				segments.push_back(segment);
			}
		}
	}

	return segments;
}

void Graph::RemoveCycleEdges(AdjacencyMatrix& graph, const VertexList& cycle)
{
	if (cycle.size() < 2)
	{
		return;
	}

	for (int i = 0; i < cycle.size() - 1; ++i)
	{
		int u = cycle[i];
		int v = cycle[i + 1];
		graph[u][v] = graph[v][u] = 0;
	}

	int last = cycle.back();
	int first = cycle.front();
	graph[last][first] = graph[first][last] = 0;
}

Segment Graph::FindSegmentWithMinFaces(const SegmentList& segments, const Lay& lay)
{
	Segment minSegment;
	int minFacesCount = std::numeric_limits<int>::max();

	for (const auto& segment : segments)
	{
		int count = 0;
		for (const Face& face : lay.GetFaces())
		{
			if (lay.AreContactsOnFace(segment.contacts, face))
			{
				count++;
			}
		}

		if (count == 0)
		{
			break;
		}

		if (count < minFacesCount)
		{
			minFacesCount = count;
			minSegment = segment;
		}
	}

	return minSegment;
}

void Graph::RemoveChainEdges(AdjacencyMatrix& graph, const VertexList& chain)
{
	for (int i = 0; i < chain.size() - 1; ++i)
	{
		int u = chain[i];
		int v = chain[i + 1];
		graph[u][v] = graph[v][u] = 0;
	}
}

TimeList Graph::DeepthFirstSearch()
{
	TimeList list(vertexCount);
	size_t time = 0;

	for (int i = 0; i < list.size(); ++i)
	{
		if (list[i].state == color::white)
		{
			DeepthFirstSearchVisit(i, list, time);
		}
	}

	return list;
}

void Graph::DeepthFirstSearchVisit(size_t i, TimeList& list, size_t& time)
{
	list[i].state = color::gray;
	list[i].entryTime = ++time;

	for (int j = 0; j < vertexCount; ++j)
	{
		if (matrix[i][j] && list[j].state == color::white)
		{
			DeepthFirstSearchVisit(j, list, time);
		}
	}

	list[i].state = color::black;
	list[i].exitTime = ++time;
}

bool Graph::DeepthFirstSearchVisit(size_t i, StateList& list, VertexList& parent, VertexList& cycle, const AdjacencyMatrix& currentMatrix)
{
	list[i] = color::gray;

	for (int j = 0; j < vertexCount; ++j)
	{
		if (!currentMatrix[i][j])
			continue;

		if (i == j && currentMatrix[i][j])
		{
			cycle.push_back(i);
			cycle.push_back(i);
			return true;
		}

		if (list[j] == color::white)
		{
			parent[j] = i;
			if (DeepthFirstSearchVisit(j, list, parent, cycle, currentMatrix))
			{
				return true;
			}
		}
		else if (list[j] == color::gray && parent[i] != j)
		{
			for (int v = i; v != j; v = parent[v])
			{
				cycle.push_back(v);
			}
			cycle.push_back(j);
			return true;
		}
	}

	list[i] = color::black;
	return false;
}

void Graph::AssertIsMatrixSize(int vertexCount)
{
	if (vertexCount <= 0)
	{
		throw std::invalid_argument("Матрица смежности не может быть пустой!");
	}
}

void Graph::AssertIsMatrixSquare(const AdjacencyMatrix& matrix)
{
	for (const auto& row : matrix)
	{
		if (row.size() != matrix.size())
		{
			throw std::invalid_argument("Матрица смежности должна быть квадратной!");
		}
	}
}

void Graph::Print() const
{
	std::cout << "Граф в виде матрицы смежности:\n";
	for (int i = 0; i < matrix.size(); ++i)
	{
		for (int j = 0; j < matrix.size(); ++j)
		{
			std::cout << matrix[i][j] << " ";
		}
		std::cout << "\n";
	}
}