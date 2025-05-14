#include "graph.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>

Graph::Graph(const AdjacencyMatrix& matrix)
	: matrix(matrix)
{
	AssertIsMatrixSquare(matrix);
}

void Graph::SetValue(int i, int j, double value)
{
	matrix[i][j] = matrix[j][i] = value;
}

AdjacencyMatrix Graph::GetMatrix() const { return matrix; }

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

void Graph::PrintGraph() const
{
	std::cout << "Граф в виде матрицы смежности:\n";
	for (int i = 0; i < matrix.size(); ++i)
	{
		for (int j = 0; j < matrix.size(); ++j)
		{
			if (i == j)
			{
				std::cout << "x";
				continue;
			}
			std::cout << matrix[i][j] << " ";
		}
		std::cout << "\n";
	}
}

const AdjacencyMatrix& Graph::CreateVisibilityGraph()
{
	for (size_t i = 0; i < vertexCount; i++)
	{
		for (size_t j = 0; j < vertexCount; j++)
		{
			if (i == j)
			{
				continue;
			}

			matrix[i][j] = 1 / matrix[i][j];
		}
	}
}
