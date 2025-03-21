#include "graph.h"
#include <fstream>

std::vector<TarjanNode> Graph::CreateTarjanCopy() const
{
	return std::vector<TarjanNode>(vertexCount);
}

Graph::Graph(int numberVertices, bool isDirected)
	: vertexCount(numberVertices)
	, matrix(numberVertices, std::vector<int>(numberVertices, 0))
	, isDirected(isDirected)
{
	if (numberVertices <= 0)
	{
		throw std::invalid_argument("Неверное число узлов");
	}
}

Graph::Graph(const AdjacencyMatrix& adjacencyMatrix, bool isDirected)
	: matrix(adjacencyMatrix)
	, vertexCount(adjacencyMatrix.size())
	, isDirected(isDirected)
{
	if (vertexCount == 0)
	{
		throw std::invalid_argument("Матрица смежности не может быть пустой");
	}

	for (const auto& row : adjacencyMatrix)
	{
		if (row.size() != vertexCount)
		{
			throw std::invalid_argument("Матрица смежности должна быть квадратной");
		}
	}
}

void Graph::AssertIsCoordsInRange(int x, int y) const
{
	if (x >= vertexCount || y >= vertexCount || x < 0 || y < 0)
	{
		throw std::runtime_error("Incorrect cord. of matrix size");
	}
}

void Graph::AddEdge(int x, int y, int value)
{
	AssertIsCoordsInRange(x, y);
	matrix[x][y] = value;
	if (!isDirected)
	{
		matrix[y][x] = value;
	}
}

void Graph::RemoveEdge(int x, int y)
{
	AssertIsCoordsInRange(x, y);
	matrix[x][y] = 0;
	if (!isDirected)
	{
		matrix[y][x] = 0;
	}
}

const AdjacencyMatrix& Graph::GetMatrix(void) const
{
	return matrix;
}

AdjacencyMatrix Graph::TranspositionMatrix() const
{
	const int size = matrix.size();
	AdjacencyMatrix transMatrix(size, std::vector<int>(size, 0));
	for (size_t i; i < size; ++i)
	{
		for (size_t j; j < size; ++j)
		{
			transMatrix[j][i] = matrix[i][j];
		}
	}
	return transMatrix;
}

void Graph::Print() const
{
	std::cout << *this;
}

void Graph::DFS(int currentVertex, std::vector<bool>& visited, std::vector<int>& entryTime,
	std::vector<int>& exitTime, int& currentTime)
{
	visited[currentVertex] = true;
	entryTime[currentVertex] = currentTime++;

	for (int i = 0; i < vertexCount; ++i)
	{
		if (matrix[currentVertex][i] != 0 && !visited[i])
		{
			DFS(i, visited, entryTime, exitTime, currentTime);
		}
	}

	exitTime[currentVertex] = currentTime++;
}

void Graph::DFSWithTimestamps()
{
	std::vector<bool> visited(vertexCount, false);
	std::vector<int> entryTime(vertexCount, -1);
	std::vector<int> exitTime(vertexCount, -1);
	int time = 0;

	// Запуск dfs с непосещенных вершин
	for (size_t i = 0; i < vertexCount; ++i)
	{
		if (!visited[i])
		{
			DFS(i, visited, entryTime, exitTime, time);
		}
	}

	std::cout << "Вершина\tВход (t)\tВыход (t)\n";
	for (size_t i = 0; i < vertexCount; ++i)
	{
		std::cout << i << "\t" << entryTime[i] << "\t" << exitTime[i] << "\n";
	}
}

Graph Graph::GetTransposedGraph() const
{
	Graph transposedGraph(vertexCount);
	transposedGraph.matrix = TranspositionMatrix();
	return transposedGraph;
}

Components Graph::GetStronglyComponentsTarjan()
{
	Components components;
	TarjanMatrix matrix = CreateTarjanCopy();
	std::stack<int> stack;
	int index = 0;

	for (size_t vertex = 0; vertex < vertexCount; vertex++)
	{
		if (matrix[vertex].index == -1)
		{
			StrongConnect(vertex, matrix, stack, index, components);
		}
	}
	return components;
}

void Graph::StrongConnect(int currentVertex, TarjanMatrix& matrix, std::stack<int>& stack, int& index, Components& components)
{
	matrix[currentVertex].index = matrix[currentVertex].lowLink = index++;
	stack.push(currentVertex);
	matrix[currentVertex].onStack = true;

	// Вот тут подумать
	for (size_t nextVertex = 0; nextVertex < vertexCount; nextVertex++)
	{
		if (this->matrix[currentVertex][nextVertex] == 0)
		{
			continue;
		}

		if (matrix[nextVertex].index == -1)
		{
			StrongConnect(nextVertex, matrix, stack, index, components);
			matrix[currentVertex].lowLink = std::min(matrix[currentVertex].lowLink, matrix[nextVertex].lowLink);
		}
		else if (matrix[nextVertex].onStack)
		{
			matrix[currentVertex].lowLink = std::min(matrix[currentVertex].lowLink, matrix[nextVertex].index);
		}
	}

	// Проверка чего-то
	if (matrix[currentVertex].lowLink == matrix[currentVertex].index)
	{
		Component strongComponent;
		int nextVertex;
		do
		{
			nextVertex = stack.top();
			stack.pop();
			matrix[nextVertex].onStack = false;
			strongComponent.push_back(nextVertex);
		} while (nextVertex != currentVertex);
		components.push_back(strongComponent);
	}
}

std::ostream& operator<<(std::ostream& os, const Graph& graph)
{
	for (const auto& row : graph.matrix)
	{
		for (int value : row)
		{
			os << value << ' ';
		}
		os << '\n';
	}
	return os;
}

ListEdge FileToGraphAdapter::ConvertEdgeListToMatrix(const Graph& graph)
{
	ListEdge listEdge;
	const auto& matrix = graph.GetMatrix();
	int vertexCount = matrix.size();

	for (int i = 0; i < vertexCount; ++i)
	{
		for (int j = i; j < vertexCount; ++j)
		{
			if (matrix[i][j] != 0)
			{
				listEdge.push_back({ i, j, matrix[i][j] });
			}
		}
	}

	return listEdge;
}

Graph FileToGraphAdapter::ConvertMatrixToEdgeList(const std::string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		throw std::runtime_error("Incorrect name of file" + fileName);
	}

	int matrixSize;
	if (!(file >> matrixSize) || matrixSize <= 0)
	{
		throw std::runtime_error("Invalid or missing matrix size in file: " + fileName);
	}

	Graph graph(matrixSize);

	int x, y, value;
	while (file >> x >> y >> value)
	{
		graph.AddEdge(x, y, value);
	}

	if (file.fail() && !file.eof())
	{
		throw std::runtime_error("Error while reading file: " + fileName);
	}

	return graph;
}