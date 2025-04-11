#ifndef SEGMENT_H
#define SEGMENT_H

#include <utility>
#include <vector>

using VertexList = std::vector<int>;
using EdgesList = std::vector<std::pair<int, int>>;

struct Segment
{
	VertexList vertices;
	VertexList contacts;
	EdgesList edges;
};

#endif // SEGMENT_H
