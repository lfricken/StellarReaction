#include "Player.hpp"
#include "Globals.hpp"
#include "Overlay.hpp"
#include "Panel.hpp"
#include "Universe.hpp"
#include "IOManager.hpp"
#include "Convert.hpp"
#include "LinearMeter.hpp"
#include "Minimap.hpp"
#include "Chunk.hpp"
#include <iostream>

Minimap::Minimap(const MinimapData& rData) : QuadComponent(rData)
{
	map_points = List<sptr<QuadComponent> >();
}
Minimap::~Minimap()
{

}
void Minimap::setDot(Vec2 center, int index, sf::Color dotColor)
{
	if(index >= (signed)map_points.size())
	{
		QuadComponentData atemp;
		atemp.dimensions = sf::Vector2f(10, 10);
		atemp.layer = GraphicsLayer::OverlayMiddle;
		atemp.color = sf::Color::White;
		map_points.push_back(sptr<QuadComponent>(new QuadComponent(atemp)));
	}

	map_points[index]->setPosition(center);
	map_points[index]->setColor(dotColor);
}
sptr<QuadComponent> Minimap::getDot(int index)
{
	return map_points[index];
}
void Minimap::cleanMap(int index)
{
	if(index < (signed)map_points.size())
		for(int i = index; i < (signed)map_points.size(); i++)
			map_points[i]->setPosition(Vec2(-10.0f, 10.0f));
}

