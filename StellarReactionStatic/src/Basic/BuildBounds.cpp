#include "BuildBounds.hpp"


BuildBounds::BuildBounds()
{

}
BuildBounds::~BuildBounds()
{

}
bool BuildBounds::contains(const Vec2& point)
{
	float dist = (point - center).len();

	if(dist < radius)
		return true;

	return false;
}
