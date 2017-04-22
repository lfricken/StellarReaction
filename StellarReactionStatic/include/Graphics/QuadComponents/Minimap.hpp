#pragma once

#include "QuadComponent.hpp"

/// Data to initalize Minimap.
struct MinimapData : public QuadComponentData
{
	MinimapData() :
		QuadComponentData()
	{
		texName = "default";
		color = sf::Color::Black;
	}
};

/// The Minimap that displays nearby objects.
class Minimap : public QuadComponent
{
public:
	Minimap(const MinimapData& rData);
	virtual ~Minimap();

	/// \brief Set a particular dot at the given position with a color.
	///
	/// index indicates the dot count.
	/// teamFlag has values -1,0,1 for Neutral, Enemy, Friendly.
	void setDot(Vec2 center, int index, sf::Color dotColor);
	/// Clear the map of dots by putting the dots off map.
	void cleanMap(int index);

	/// \brief Get a particular dot from the minimap.
	///
	/// index indicates the dot count.
	sptr<QuadComponent> getDot(int index);
protected:
	List<sptr<QuadComponent> > map_points;
private:
};
