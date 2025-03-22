#include "../class-graph/graph.h"
#include <gtest/gtest.h>

TEST(GraphTest, AddEdgeUndirected)
{
	Graph g(3, false);
	g.AddEdge(0, 1);

	const auto& matrix = g.GetMatrix();
	EXPECT_EQ(matrix[0][1], 1);
	EXPECT_EQ(matrix[1][0], 1);
}

TEST(GraphTest, AddEdgeDirected)
{
	Graph g(3, true);
	g.AddEdge(0, 1);

	const auto& matrix = g.GetMatrix();
	EXPECT_EQ(matrix[0][1], 1);
	EXPECT_EQ(matrix[1][0], 0);
}

TEST(GraphTest, DFSWithTimestamps)
{
	Graph g(4, false);
	g.AddEdge(0, 1);
	g.AddEdge(1, 2);
	g.AddEdge(2, 3);

	auto timestamps = g.DFSWithTimestamps();

	EXPECT_EQ(timestamps.size(), 4);
	for (const auto& node : timestamps)
	{
		EXPECT_NE(node.entryTime, -1);
		EXPECT_NE(node.exitTime, -1);
	}
}

TEST(GraphTest, GetStrongComponentsKosaraju)
{
	AdjacencyMatrix matrix = {
		{0, 1, 0, 0, 0},
		{0, 0, 1, 1, 0},
		{1, 0, 0, 0, 0},
		{0, 0, 0, 0, 1},
		{0, 0, 0, 1, 0}};

	Graph g(matrix, true);
	Components components = g.GetStrongComponentsKosaraju();

	ASSERT_EQ(components.size(), 2);
	EXPECT_EQ(components[0].size(), 3); // {0, 2, 1}
	EXPECT_EQ(components[1].size(), 2); // {3, 4}
}