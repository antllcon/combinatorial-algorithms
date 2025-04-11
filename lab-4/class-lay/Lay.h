#ifndef LAY_H
#define LAY_H

#include "../Segment.h"
#include <vector>

using Face = std::vector<int>;
using FaceList = std::vector<Face>;
using ChainList = std::vector<VertexList>;

class Lay
{
public:
	Lay();
	~Lay() = default;
	FaceList GetFaces() const;
	void AddInitialCycle(const VertexList& cycle);
	FaceList FindSuitableFaces(const Segment& segment) const;
	bool CanEmbedSegmentInFace(const Segment& segment, const Face& face) const;
	bool AreContactsOnFace(const VertexList& contacts, const Face& face) const;
	VertexList FindChainBetweenContacts(const Segment& segment, const Face& targetFace) const;
	void EmbedPath(const VertexList& path, const Face& targetFace);

private:
	FaceList faces;

	ChainList SplitSegmentChains(const Segment& segment, const Face& face) const;
	bool CanEmbedChainInFace(const VertexList& chain, const Face& face) const;
	bool CanEmbedComplexSegment(const Segment& segment, const Face& face) const;
	bool IsSimpleChain(const Segment& segment) const;
};

#endif // LAY_H
