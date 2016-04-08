#include "Player.hpp"
#include "Globals.hpp"
#include "Overlay.hpp"
#include "Panel.hpp"
#include "Universe.hpp"
#include "IOManager.hpp"
#include "Convert.hpp"
#include "DecorQuad.hpp"
#include "LinearMeter.hpp"
#include "Minimap.hpp"
#include "Chunk.hpp"
#include <iostream>

using namespace std;

Minimap::Minimap(const MinimapData& rData) : QuadComponent(rData)
{
	m_controller = rData.controller;
	map_points = std::vector<sptr<QuadComponent> >();
}
Minimap::~Minimap()
{
}
void Minimap::setDot(b2Vec2 center, int index, int team)
{
	if(index >= (signed)map_points.size())
	{
		QuadComponentData atemp;
		atemp.dimensions = sf::Vector2f(10, 10);
		atemp.layer = GraphicsLayer::OverlayMiddle;
		if (team == 1)
			atemp.color = sf::Color::Green;
		else if (team == 0)
			atemp.color = sf::Color::Red;
		else
			atemp.color = sf::Color::Blue;
		QuadComponent* btemp = new QuadComponent(atemp);
		btemp->setPosition(center);
		sptr<QuadComponent> new_val = sptr<QuadComponent>(btemp);
		map_points.push_back(new_val);
	}
	else
	{
		map_points[index]->setPosition(center);
		if (team == 1)
			map_points[index]->setColor(sf::Color::Green);
		else if (team == 0)
			map_points[index]->setColor(sf::Color::Red);
		else
			map_points[index]->setColor(sf::Color::Blue);
	}
}
void Minimap::cleanMap(int index)
{
	if(index < (signed)map_points.size())
	{
		for(int i = index; i < (signed)map_points.size(); i++)
		{
			map_points[i]->setPosition(b2Vec2(-10.0f, 10.0f));
		}
	}
}