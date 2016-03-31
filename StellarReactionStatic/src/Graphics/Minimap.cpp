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
	map_points = std::vector<sptr<DecorQuad> >();
}
Minimap::~Minimap()
{
}
void Minimap::setDot(b2Vec2 center, int index)
{
	if(index >= (signed)map_points.size())
	{
		DecorQuadData temp;
		temp.quadComp.dimensions = sf::Vector2f(10, 10);
		temp.quadComp.layer = GraphicsLayer::OverlayMiddle;
		temp.quadComp.color = sf::Color::Red;
		DecorQuad* ptemp = new DecorQuad(temp);
		ptemp->setPosition(center);
		sptr<DecorQuad> new_value = sptr<DecorQuad>(ptemp);
		map_points.push_back(new_value);
	}
	else
		map_points[index]->setPosition(center);
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