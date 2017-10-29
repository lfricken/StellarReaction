
#include "stdafx.hpp"

List<Vec2> taxicabCircle(int radius)
{
	List<Vec2> coordinates;

	for(int i = 1; i < radius; ++i)//skip the 4 corner coordinates (otherwise they get repeated
	{
		coordinates.push_back(Vec2((float)i, (float)radius - i));
	}

	{//copy, mirror across y axis, and add.
		auto topLeft(coordinates);
		for(auto it = topLeft.begin(); it != topLeft.end(); ++it)
		{
			it->x = -it->x;
		}
		coordinates.insert(coordinates.end(), topLeft.begin(), topLeft.end());
	}
	{//copy, mirror across x axis, and add.
		auto bottom(coordinates);
		for(auto it = bottom.begin(); it != bottom.end(); ++it)
		{
			it->y = -it->y;
		}
		coordinates.insert(coordinates.end(), bottom.begin(), bottom.end());
	}
	{//finally add the last 4 coordinates
		coordinates.push_back(Vec2((float)radius, 0.f));
		coordinates.push_back(Vec2((float)-radius, 0.f));
		coordinates.push_back(Vec2(0.f, (float)radius));
		coordinates.push_back(Vec2(0.f, (float)-radius));
	}
	return coordinates;
}




