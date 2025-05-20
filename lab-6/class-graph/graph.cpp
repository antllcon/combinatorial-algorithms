#include "graph.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>

Graph::Graph(const Matrix& matrix)
	: m_matrix(matrix)
{
}

Graph::Graph(int vetexCount, double value)
	: m_matrix(vetexCount, value)
{
}

std::vector<double>& Graph::operator[](int row)
{
	return m_matrix[row];
}

const std::vector<double>& Graph::operator[](int row) const
{
	return m_matrix[row];
}

int Graph::GetSize() const
{
	return m_matrix.GetSize();
}

Matrix Graph::GetMatrix() const
{
	return m_matrix;
}

void Graph::PrintGraph() const
{
	m_matrix.Print();
}

bool Graph::IsEmpty() const
{
	return m_matrix.IsEmpty();
}

Graph FileToGraphAdapter::ConvertEdgeListToMatrix(const std::string& fileName)
{
	std::ifstream file(fileName);
	AssertIsFileOpen(file, fileName);
	AssertIsStreamCorrect(file);

	size_t matrixSize = SafeRead<size_t>(file, "Размер матрицы?");
	AssertIsValidSize(matrixSize);

	Matrix matrix = ReadMatrix(file, matrixSize);
	Graph graph(matrix);
	return graph;
}

Matrix FileToGraphAdapter::ReadMatrix(std::ifstream& file, size_t matrixSize) const
{
	Matrix matrix(matrixSize, 0.0);

	size_t from, to, size;
	std::string line;
	while (file >> from >> line >> to >> size)
	{
		from--;
		to--;
		AssertIsValidNumbers(from, to, matrixSize);
		matrix[from][to] = matrix[to][from] = size;
	}

	AssertIsStreamCorrect(file);
	return matrix;
}
template <typename T>
T FileToGraphAdapter::SafeRead(std::ifstream& file, const std::string& errorMessage)
{
	T value;
	if (!(file >> value))
	{
		throw std::runtime_error(errorMessage);
	}
	return value;
}

void FileToGraphAdapter::AssertIsFileOpen(
	std::ifstream& file, const std::string& fileName) const
{
	if (!file.is_open())
	{
		std::cout << fileName << std::endl;
		throw std::runtime_error("Невозможно открыть файл: " + fileName);
	}
}

void FileToGraphAdapter::AssertIsStreamCorrect(std::ifstream& file) const
{
	if (file.fail() && !file.eof())
	{
		throw std::runtime_error("Ошибка чтения данных из файла");
	}
}

void FileToGraphAdapter::AssertIsValidSize(size_t size) const
{
	if (size <= 0)
	{
		throw std::runtime_error("Некорректный размер матрицы");
	}
}

void FileToGraphAdapter::AssertIsValidNumbers(
	size_t from, size_t to, size_t matrixSize) const
{
	if (from >= matrixSize || to >= matrixSize)
	{
		throw std::runtime_error("Некорректные номера вершин");
	}
}
