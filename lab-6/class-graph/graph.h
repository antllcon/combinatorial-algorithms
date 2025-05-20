#ifndef GRAPH_H
#define GRAPH_H

#include "matrix.h"
#include <fstream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

using VertexList = std::vector<int>;

class Graph
{
public:
	Graph(const Matrix& matrix);
	Graph(int vetexCount, double value);
	~Graph() = default;

	std::vector<double>& operator[](int row);
	const std::vector<double>& operator[](int row) const;

	int GetSize() const;
	Matrix GetMatrix() const;
	void PrintGraph() const;
	bool IsEmpty() const;

private:
	Matrix m_matrix;
};

using Vertex = std::pair<int, int>;
using Coordinates = std::vector<Vertex>;

/// @brief Класс адаптер
class FileToGraphAdapter
{
public:
	Graph ConvertEdgeListToMatrix(const std::string& fileName);

private:
	Matrix ReadMatrix(std::ifstream& file, size_t matrixSize) const;

	template <typename T>
	static T SafeRead(std::ifstream& file, const std::string& errorMessage);
	void AssertIsFileOpen(std::ifstream& file, const std::string& fileName) const;
	void AssertIsStreamCorrect(std::ifstream& file) const;
	void AssertIsValidSize(size_t size) const;
	void AssertIsValidNumbers(size_t from, size_t to, size_t matrixSize) const;
};

#endif // GRAPH_H