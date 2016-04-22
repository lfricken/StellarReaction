#pragma once

#include "stdafx.hpp"
#include "Timer.hpp"
#include "IOComponent.hpp"

class Universe;

/*
* HazardField Class:
* Implements HazardField object
* HazardField object defines a utility to create hazard fields in the game universe.
*/

class HazardField
{

public:
	HazardField(Universe* universe, const Json::Value& root);
	///Spawns a hazard in a random location within the field.
	void spawn();
	///Recieve commands.
	void input(std::string command, sf::Packet data);
private:
	Universe* m_pUniverse;
	std::string m_hazardName;
	int m_numHazards;
	float m_radius;
	b2Vec2 m_origin;

	IOComponent m_io;
};


