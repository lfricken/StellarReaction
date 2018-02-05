#include "BuildBounds.hpp"


BuildBounds::BuildBounds()
{

}
BuildBounds::~BuildBounds()
{

}
bool BuildBounds::contains(const Vec2& point)
{
	Vec2 minMaxX(center.x - halfSize.x, center.x + halfSize.x);
	Vec2 minMaxY(center.y - halfSize.y, center.y + halfSize.y);

	if(point.x > minMaxX.x && point.x < minMaxX.y)
		if(point.y > minMaxY.x && point.y < minMaxY.y)
			return true;

	return false;
}
