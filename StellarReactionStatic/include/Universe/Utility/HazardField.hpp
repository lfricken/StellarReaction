#pragma once

#include "stdafx.hpp"
#include "Timer.hpp"
#include "IOComponent.hpp"

class Universe;

/// Used briefly to create a collection of Hazards in the world.
class HazardField
{

public:
	HazardField(Universe* universe, const Json::Value& root);
	///Spawns a hazard in a random location within the field.
	void spawn();
	///Recieve commands.
	void input(String command, sf::Packet data);
private:
	Universe* m_pUniverse;
	String m_hazardName;
	int m_numHazards;
	float m_radius;
	Vec2 m_origin;

	IOComponent m_io;
};


