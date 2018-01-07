#include "CalculateArea.h"
#include <cmath>
#include <cstdint>
double CalculateArea(const Polygon &polygon)
{
    int64_t area=0;
	for(auto point=polygon.begin();point!=polygon.end();++point)
	{
		auto next=point+1;
		if(next==polygon.end())
			next=polygon.begin();
        area+=
            int64_t(point->x())*int64_t(next->y())
            -int64_t(point->y())*int64_t(next->x());
	}
    return static_cast<double>(std::abs(area));
}
