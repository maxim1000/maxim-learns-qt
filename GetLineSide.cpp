#include "GetLineSide.h"
#include <cstdint>
int GetLineSide(const QPoint &point,const Line &line)
{
	const auto vector1=point-line.first;
	const auto vector2=line.second-line.first;
	const auto crossProduct=
		int64_t(vector1.x())*int64_t(vector2.y())
		-int64_t(vector1.y())*int64_t(vector2.x());
	if(crossProduct<0)
		return -1;
	else if(crossProduct>0)
		return 1;
	else
		return 0;
}
