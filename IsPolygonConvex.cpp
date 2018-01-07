#include "IsPolygonConvex.h"
#include "GetLineSide.h"
#include <algorithm>
#include <fstream>
bool IsPolygonConvex(const Polygon &polygon)
{
	std::vector<int> signs;
    for(auto point1=polygon.begin();point1!=polygon.end();++point1)
    {
        auto point2=point1+1;
        if(point2==polygon.end())
            point2=polygon.begin();
        auto point3=point2+1;
        if(point3==polygon.end())
            point3=polygon.begin();
        signs.push_back(GetLineSide(*point3,QLine(*point1,*point2)));
    }
    const bool negative=(std::find(signs.begin(),signs.end(),-1)!=signs.end());
    const bool positive=(std::find(signs.begin(),signs.end(),1)!=signs.end());
    return !negative || !positive;
}
