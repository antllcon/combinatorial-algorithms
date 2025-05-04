#include "../class-graph/Graph.h"
#include <gtest/gtest.h>

class GraphTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		// Простой цикл из 3 вершин
		adjacencyMatrix3 = {{0, 1, 1, 0, 0}, {1, 0, 1, 1, 1}, {1, 1, 0, 0, 0}, {0, 1, 0, 0, 1}, {0, 1, 0, 1, 0}};

		// Полный граф K4
		adjacencyMatrixK4 = {{0, 1, 1, 1}, {1, 0, 1, 1}, {1, 1, 0, 1}, {1, 1, 1, 0}};

		// Граф с двумя компонентами связности
		adjacencyMatrixTwoComponents = {{0, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}};

		// Граф с wiki
		adjacencyMatrixPlanar = {
			{0, 1, 0, 0, 0, 1, 0},
			{1, 0, 1, 1, 1, 0, 0},
			{0, 1, 0, 1, 1, 0, 0},
			{0, 1, 1, 0, 1, 0, 0},
			{0, 1, 1, 1, 0, 1, 1},
			{1, 0, 0, 0, 1, 0, 1},
			{0, 0, 0, 0, 1, 1, 0},
		};

		adjacencyMatrixPlanarNoCycle
			= {{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0}, {0, 0, 0, 0, 1, 0, 0}, {0, 1, 0, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0, 1}, {0, 0, 0, 0, 0, 0, 1}, {0, 0, 0, 0, 1, 1, 0}};

		adjacencyMatrixSimpleCycle = {{0, 1, 0, 0, 0}, {1, 0, 1, 1, 0}, {0, 1, 0, 1, 1}, {0, 1, 1, 0, 0}, {0, 0, 1, 0, 0}};

		bestExample = {{0, 1, 0, 1, 0}, {1, 0, 1, 0, 1}, {0, 1, 0, 1, 1}, {1, 0, 1, 0, 0}, {0, 1, 1, 0, 0}};

		bestExampleUpdate = {{0, 1, 0, 1, 0, 0, 0, 1},
			{1, 0, 1, 0, 1, 0, 0, 0},
			{0, 1, 0, 1, 1, 0, 0, 0},
			{1, 0, 1, 0, 0, 0, 0, 1},
			{0, 1, 1, 0, 0, 1, 1, 0},
			{0, 0, 0, 0, 1, 0, 1, 0},
			{0, 0, 0, 0, 1, 1, 0, 0},
			{1, 0, 0, 1, 0, 0, 0, 0}};

		bestExampleUpdateNoCycle = {{0, 0, 0, 0, 0, 0, 0, 1},
			{0, 0, 0, 0, 1, 0, 0, 0},
			{0, 0, 0, 0, 1, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 1},
			{0, 1, 1, 0, 0, 1, 1, 0},
			{0, 0, 0, 0, 1, 0, 1, 0},
			{0, 0, 0, 0, 1, 1, 0, 0},
			{1, 0, 0, 1, 0, 0, 0, 0}};
	};
	AdjacencyMatrix bestExampleUpdateNoCycle;
	AdjacencyMatrix bestExampleUpdate;
	AdjacencyMatrix bestExample;
	AdjacencyMatrix adjacencyMatrix3;
	AdjacencyMatrix adjacencyMatrixK4;
	AdjacencyMatrix adjacencyMatrixTwoComponents;
	AdjacencyMatrix adjacencyMatrixPlanar;
	AdjacencyMatrix adjacencyMatrixPlanarNoCycle;
	AdjacencyMatrix adjacencyMatrixSimpleCycle;
};

// TEST_F(GraphTest, GetCycleTest)
// {
// 	AdjacencyMatrix matrix = {{0, 1, 0, 0, 0, 0, 0}, {1, 0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 1, 1}, {0, 1, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0, 1}, {0, 0, 1, 0, 0, 1, 0}};
// 	Graph graph(matrix);
// 	VertexList cycle = graph.GetCycle(graph.GetMatrix());

// 	std::cout << "Цикл:\n";
// 	for (auto val : cycle)
// 		std::cout << val + 1 << " ";
// 	std::cout << std::endl << std::endl;

// 	ASSERT_EQ(cycle.size(), 3);
// 	// EXPECT_EQ(adjacencyMatrix3[cycle[0]][cycle[1]], 1);
// 	// EXPECT_EQ(adjacencyMatrix3[cycle[1]][cycle[2]], 1);
// 	// EXPECT_EQ(adjacencyMatrix3[cycle[2]][cycle[0]], 1);
// }

// TEST_F(GraphTest, AddInitialCycleTest)
// {
// 	Lay lay;

// 	VertexList cycle = {1, 2, 3, 4};
// 	lay.AddInitialCycle(cycle);

// 	FaceList faces = lay.GetFaces();

// 	ASSERT_EQ(faces.size(), 2);
// 	EXPECT_EQ(faces[0][0], 1);
// 	EXPECT_EQ(faces[0][1], 2);
// 	EXPECT_EQ(faces[0][2], 3);
// 	EXPECT_EQ(faces[0][3], 4);

// 	EXPECT_EQ(faces[1][0], 4);
// 	EXPECT_EQ(faces[1][1], 3);
// 	EXPECT_EQ(faces[1][2], 2);
// 	EXPECT_EQ(faces[1][3], 1);
// }

// TEST_F(GraphTest, RemoveCycleEdgesTest)
// {
// 	AdjacencyMatrix matrix = adjacencyMatrixPlanar;
// 	Graph graph(matrix);
// 	VertexList cycle = {0, 1, 2, 3, 4, 5};
// 	graph.RemoveCycleEdges(matrix, cycle);

// 	EXPECT_EQ(matrix[0][1], 0);
// 	EXPECT_EQ(matrix[1][2], 0);
// 	EXPECT_EQ(matrix[2][3], 0);
// 	EXPECT_EQ(matrix[3][4], 0);
// 	EXPECT_EQ(matrix[4][5], 0);
// 	EXPECT_EQ(matrix[5][0], 0);
// 	EXPECT_EQ(matrix[0][3], 0);
// 	EXPECT_EQ(matrix[1][3], 1);
// 	EXPECT_EQ(matrix[1][4], 1);
// 	EXPECT_EQ(matrix[2][4], 1);
// }

// TEST_F(GraphTest, RemoveChainEdgesTest)
// {
// 	AdjacencyMatrix matrix = adjacencyMatrixPlanar;
// 	Graph graph(matrix);
// 	VertexList chain = {0, 1, 2, 3, 4, 5};
// 	graph.RemoveChainEdges(matrix, chain);

// 	EXPECT_EQ(matrix[0][1], 0);
// 	EXPECT_EQ(matrix[1][2], 0);
// 	EXPECT_EQ(matrix[2][3], 0);
// 	EXPECT_EQ(matrix[3][4], 0);
// 	EXPECT_EQ(matrix[4][5], 0);
// 	EXPECT_EQ(matrix[0][3], 0);
// 	EXPECT_EQ(matrix[1][3], 1);
// 	EXPECT_EQ(matrix[1][4], 1);
// 	EXPECT_EQ(matrix[2][4], 1);
// }

// TEST_F(GraphTest, FindContactVerticesTest)
// {
// 	AdjacencyMatrix matrix = bestExampleUpdate;
// 	Graph graph(matrix);
// 	VertexList planarGraph = {0, 1, 2, 3};

// 	VertexList contacts = graph.FindContactVertices(graph.GetMatrix(), planarGraph);
// 	VertexList expectedContacts = {0, 1, 2, 3};
// 	ASSERT_EQ(contacts.size(), expectedContacts.size());
// 	EXPECT_EQ(std::unordered_set<int>(contacts.begin(), contacts.end()), std::unordered_set<int>(expectedContacts.begin(), expectedContacts.end()));
// }

// TEST_F(GraphTest, FindSegmentsTest)
// {
// 	AdjacencyMatrix matrix = bestExampleUpdateNoCycle;
// 	Graph graph(matrix);
// 	VertexList contactVertecies = {0, 1, 2, 3};

// 	SegmentList list = graph.FindSegments(graph.GetMatrix(), contactVertecies);

// 	std::cout << "\nВсе найденые сегменты:\n";
// 	for (auto segment : list)
// 	{
// 		std::cout << "[";
// 		for (auto vertex : segment.vertices)
// 		{
// 			std::cout << vertex << " ";
// 		}
// 		std::cout << "]\n";
// 	}
// }

TEST_F(GraphTest, IsPlanarGraphReturnsTrueForK3)
{
	Graph graph(adjacencyMatrix3);
	EXPECT_TRUE(graph.IsPlanarGraph());
}

TEST_F(GraphTest, Normal)
{
	// Непланарный граф (K5)
	AdjacencyMatrix matrix = bestExampleUpdate;

	Graph graph(matrix);
	ASSERT_TRUE(graph.IsPlanarGraph());
}

TEST_F(GraphTest, Normal)
{
	AdjacencyMatrix matrix = bestExampleUpdate;

	Graph graph(matrix);
	ASSERT_TRUE(graph.IsPlanarGraph());
}
