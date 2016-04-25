#pragma once

#include "QuadComponent.hpp"

/// Data to initalize Minimap.
struct MinimapData : public QuadComponentData
{
	MinimapData() :
		QuadComponentData()
	{
		texName = "default.png";
		color = sf::Color::Black;
	}
};

/// The Minimap that displays nearby objects.
class Minimap : public QuadComponent
{
public:
	Minimap(const MinimapData& rData);
	virtual ~Minimap();

	/// Set a dot at the given position.
	/// index indicates the dot count.
	/// teamFlag has values -1,0,1 for Neutral, Enemy, Friendly.
	void setDot(b2Vec2 center, int index, int teamFlag);
	/// Clear the map of dots by putting the dots off map.
	void cleanMap(int index);

protected:
	std::vector<sptr<QuadComponent> > map_points;
private:
};
