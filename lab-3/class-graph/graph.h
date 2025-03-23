#include <iostream>
#include <stack>
#include <vector>

enum class color
{
	white, // Не посещенная
	gray,  // Посещенная (не завершенная)
	black  // Посещенная (завершенная)
};

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

struct TimeNode
{
	color visited = color::white;
	int entryTime = -1;
	int exitTime = -1;
};

using AdjacencyMatrix = std::vector<std::vector<int>>;
using TarjanMatrix = std::vector<TarjanNode>;
using TimeMatrix = std::vector<TimeNode>;

using Component = std::vector<int>;
using Components = std::vector<Component>;
using ListEdge = std::vector<Edge>;

class Graph
{
	AdjacencyMatrix matrix;
	int vertexCount;
	bool isDirected;

	TarjanMatrix CreateTarjanCopy() const;
	TimeMatrix CreateTimeCopy() const;
	void AssertIsCoordsInRange(int x, int y) const;
	Component ReverseSortExitTime(TimeMatrix& matrix) const;
	AdjacencyMatrix TranspositionMatrix(const AdjacencyMatrix& matrix) const;

	void DFS(int i, const AdjacencyMatrix& matrix, TimeMatrix& TimeMatrix, int& currentTime, Component* component);
	Components DFSWithCollectComponents(const Component& sortedVertexes, AdjacencyMatrix& transpositionMatrix);

public:
	Graph(int numberVertices, bool isDirected = false);
	Graph(const AdjacencyMatrix& adjacencyMatrix, bool isDirected = false);

	void AddEdge(int from, int to, int value = 1);
	void RemoveEdge(int from, int to);
	Graph GetInvertGraph() const;
	const AdjacencyMatrix& GetMatrix(void) const;

	TimeMatrix DFSWithTimestamps();
	Components GetStrongComponentsKosaraju();

	friend std::ostream& operator<<(std::ostream& os, const Graph& graph);
};

std::ostream& operator<<(std::ostream& os, const Components& components);

class FileToGraphAdapter
{
	static ListEdge ConvertEdgeListToMatrix(const Graph& graph);
	static Graph ConvertMatrixToEdgeList(const std::string& fileName);
};