#include "shapeUtil.h"

namespace core
{

float Evaluate(const Line& line, float x)
{
	return line.slope * x + line.intercept;
}

Line ToLine(const Segment& seg)
{
	Line line;
	if (std::abs(seg.end.x - seg.start.x) < 1e-6)
	{
		return line;
	}
	line.slope = (seg.end.y - seg.start.y) / (seg.end.x - seg.start.x);
	line.intercept = seg.start.y - line.slope * seg.start.x;
	return line;
}

}	 // namespace core