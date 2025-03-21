#include <iostream>
#include <stack>
#include <vector>

struct Edge
{
	int from;
	int to;
	int value;
};

struct TarjanNode
{
	int index = -1;
	int lowLink = -1;
	bool onStack = false;
};

using AdjacencyMatrix = std::vector<std::vector<int>>;
using TarjanMatrix = std::vector<TarjanNode>;
using Component = std::vector<int>;
using Components = std::vector<Component>;
using ListEdge = std::vector<Edge>;

class Graph
{
	AdjacencyMatrix matrix;
	int vertexCount;
	bool isDirected;

	void AssertIsCoordsInRange(int x, int y) const;
	AdjacencyMatrix TranspositionMatrix() const;
	void DFS(int currentVertex, std::vector<bool>& visited, std::vector<int>& entryTime,
		std::vector<int>& exitTime, int& currentTime);
	std::vector<TarjanNode> Graph::CreateTarjanCopy() const;
	void Graph::StrongConnect(int currentVertex, TarjanMatrix& matrix, std::stack<int>& stack, int& index, Components& components);

public:
	Graph(int numberVertices, bool isDirected = false);
	Graph(const AdjacencyMatrix& adjacencyMatrix, bool isDirected = false);
	void Print() const;
	void AddEdge(int x, int y, int value);
	void RemoveEdge(int x, int y);
	const AdjacencyMatrix& GetMatrix(void) const;
	Graph GetTransposedGraph() const;
	Components GetStronglyComponentsTarjan();
	Components GetStronglyComponentsKosaraju();
	friend std::ostream& operator<<(std::ostream& os, const Graph& graph);
	void DFSWithTimestamps();
};

class FileToGraphAdapter
{
	static ListEdge ConvertEdgeListToMatrix(const Graph& graph);
	static Graph ConvertMatrixToEdgeList(const std::string& fileName);
};