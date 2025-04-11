#ifndef GRAPH_H
#define GRAPH_H

#include "../Segment.h"
#include "../class-lay/Lay.h"
#include <gtest/gtest_prod.h>
#include <stack>
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

using AdjacencyMatrix = std::vector<std::vector<int>>;
using TimeList = std::vector<TimeVertex>;
using StateList = std::vector<color>;
using SegmentList = std::vector<Segment>;
using ChainList = std::vector<VertexList>;

class Graph
{
	// Объявляем тесты как дружественные
	FRIEND_TEST(GraphTest, RemoveCycleEdgesTest);
	FRIEND_TEST(GraphTest, RemoveChainEdgesTest);
	FRIEND_TEST(GraphTest, FindContactVerticesTest);
	FRIEND_TEST(GraphTest, FindSegmentsTest);
	FRIEND_TEST(GraphTest, FindComponentBFSTest);
	FRIEND_TEST(GraphTest, FindSegmentWithMinFacesTest);

public:
	Graph(const AdjacencyMatrix& matrix);
	~Graph() = default;

	AdjacencyMatrix GetMatrix() const;
	int GetVertexCount() const;
	TimeList DeepthFirstSearch();
	VertexList GetCycle(const AdjacencyMatrix& matrix);
	bool IsPlanarGraph();
	void Print() const;

private:
	AdjacencyMatrix matrix;
	int vertexCount;

	void AddPath(VertexList& planarGraph, const VertexList& path);
	void RemoveCycleEdges(AdjacencyMatrix& graph, const VertexList& cycle);
	void RemoveChainEdges(AdjacencyMatrix& graph, const VertexList& chain);

	Segment FindConnectedComponent(int start, const AdjacencyMatrix& graph, const VertexList& contacts, std::unordered_set<int>& visited);
	VertexList FindContactVertices(const AdjacencyMatrix& fullGraph, const VertexList& planarSubgraph);
	SegmentList FindSegments(const AdjacencyMatrix& workingGraph, const VertexList& planarVertices);
	Segment FindSegmentWithMinFaces(const SegmentList& segments, const Lay& lay);
	void DeepthFirstSearchVisit(size_t number, TimeList& list, size_t& time);
	bool DeepthFirstSearchVisit(size_t i, StateList& list, VertexList& parent, VertexList& cycle, const AdjacencyMatrix& currentMatrix);

	void AssertIsMatrixSize(int vertexCount);
	void AssertIsMatrixSquare(const AdjacencyMatrix& matrix);
};

#endif // GRAPH_H
