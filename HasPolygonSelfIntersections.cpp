#include "HasPolygonSelfIntersections.h"
#include "GetLineSide.h"
namespace
{
	using Segment=std::pair<QPoint,QPoint>;
    bool DoesSegmentIntersectLine(const Segment &segment,const QLine &line)
	{
        const auto side1=GetLineSide(segment.first,line);
        const auto side2=GetLineSide(segment.second,line);
		return side1*side2<=0;
	}
	bool DoSegmentsIntersect(const Segment &segment1,const Segment &segment2)
	{
		return
            DoesSegmentIntersectLine(segment1,QLine(segment2.first,segment2.second))
            && DoesSegmentIntersectLine(segment2,QLine(segment1.first,segment1.second));
	}
}
bool HasPolygonSelfIntersections(const Polygon &polygon)
{
    using PolygonEdge=std::pair<Polygon::const_iterator,Polygon::const_iterator>;
	std::vector<PolygonEdge> edges;
	for(auto point=polygon.begin();point!=polygon.end();++point)
	{
		auto next=point+1;
		if(next==polygon.end())
			next=polygon.begin();
		edges.emplace_back(point,next);
	}
	for(const auto &edge1:edges)
	{
		for(const auto &edge2:edges)
		{
			if(edge1.first==edge2.first)
				continue;
			if(edge1.second==edge2.first)
				continue;
			if(edge1.first==edge2.second)
				continue;
			const bool intersect=DoSegmentsIntersect(
				Segment(*edge1.first,*edge1.second),
				Segment(*edge2.first,*edge2.second));
			if(intersect)
				return true;
		}
	}
	return false;
}
