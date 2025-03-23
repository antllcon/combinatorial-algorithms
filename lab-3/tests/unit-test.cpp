#include "../class-graph/graph.h"
#include <gtest/gtest.h>

TEST(GraphTest, Constructor_ValidSize)
{
	EXPECT_NO_THROW(Graph g(5, true));
}

TEST(GraphTest, Constructor_InvalidSize)
{
	EXPECT_THROW(Graph g(0, true), std::invalid_argument);
}

TEST(GraphTest, AddEdge)
{
	Graph g(3, true);
	g.AddEdge(1, 2, 1);
	const auto& matrix = g.GetMatrix();
	EXPECT_EQ(matrix[0][1], 1);
}

TEST(GraphTest, RemoveEdge)
{
	Graph g(3, true);
	g.AddEdge(1, 2, 1);
	g.RemoveEdge(1, 2);
	const auto& matrix = g.GetMatrix();
	EXPECT_EQ(matrix[0][1], 0);
}

TEST(GraphTest, Transpose)
{
	Graph g(3, true);
	g.AddEdge(1, 2, 1);
	Graph transposed = g.GetInvertGraph();
	const auto& matrix = transposed.GetMatrix();
	EXPECT_EQ(matrix[1][0], 1);
}

TEST(GraphTest, Inv)
{
	AdjacencyMatrix matrix = {
		{0, 1, 0},
		{0, 0, 1},
		{1, 0, 0}};
	Graph graph(matrix, true);
	Graph invertGraph = graph.GetInvertGraph();
	AdjacencyMatrix transposed = invertGraph.GetMatrix();
	EXPECT_EQ(transposed[0][2], 1);
	EXPECT_EQ(transposed[1][0], 1);
	EXPECT_EQ(transposed[2][1], 1);
}

TEST(GraphTest, DFS)
{
	Graph graph(3, true);
	graph.AddEdge(1, 2, 1);
	graph.AddEdge(2, 3, 1);
	TimeMatrix matrix = graph.DFSWithTimestamps();
	EXPECT_EQ(matrix[0].visited, color::black);
	EXPECT_EQ(matrix[1].visited, color::black);
	EXPECT_EQ(matrix[2].visited, color::black);
}

TEST(GraphTest, StronglyConnectedComponents)
{
	// Граф из презентации
	AdjacencyMatrix matrix = {
		{0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

	Graph g(matrix, true);
	Components components = g.GetStrongComponentsKosaraju();

	ASSERT_EQ(components.size(), 8);
}