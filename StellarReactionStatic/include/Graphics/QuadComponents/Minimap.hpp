#ifndef MINIMAP_HPP
#define MINIMAP_HPP

#include "QuadComponent.hpp"

struct MinimapData : public QuadComponentData
{
	MinimapData() :
		controller(0),
		QuadComponentData()
	{
		texName = "default.png";
		color = sf::Color::Black;
	}

	int controller;

};

class DecorQuad;

/// <summary>
/// Something conveinent for using as a meter (for gui)
/// </summary>
class Minimap : public QuadComponent
{
public:
	Minimap(const MinimapData& rData);
	virtual ~Minimap();

	void setDot(b2Vec2 center, int index, int team);
	void cleanMap(int index);

protected:
	int m_controller;
	std::vector<sptr<QuadComponent> > map_points;
private:
};

#endif // MINIMAP_HPP