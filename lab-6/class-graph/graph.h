#ifndef GRAPH_H
#define GRAPH_H

#include "../Segment.h"
#include "../class-lay/Lay.h"
#include <fstream>
#include <gtest/gtest_prod.h>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

enum class color
{
	white,
	gray,
	black
};

struct TimeVertex
{
	color state = color::white;
	int entryTime = -1;
	int exitTime = -1;
};

using AdjacencyMatrix = std::vector<std::vector<double>>;
using TimeList = std::vector<TimeVertex>;
using StateList = std::vector<color>;

class Graph
{

public:
	Graph(const AdjacencyMatrix& matrix);
	~Graph() = default;

	void SetValue(int i, int j, double value);
	AdjacencyMatrix GetMatrix() const;
	TimeList DeepthFirstSearch();
	VertexList GetCycle(const AdjacencyMatrix& matrix);
	bool IsPlanarGraph();
	void PrintGraph() const;
	const AdjacencyMatrix& CreateVisibilityGraph();

private:
	void AddPath(VertexList& planarGraph, const VertexList& path);
	void DeepthFirstSearchVisit(size_t number, TimeList& list, size_t& time);
	bool DeepthFirstSearchVisit(size_t i,
		StateList& list,
		VertexList& parent,
		VertexList& cycle,
		const AdjacencyMatrix& currentMatrix);

	void AssertIsMatrixSize(int vertexCount);
	void AssertIsMatrixSquare(const AdjacencyMatrix& matrix);

private:
	AdjacencyMatrix matrix;
	int vertexCount;
};

#endif // GRAPH_H