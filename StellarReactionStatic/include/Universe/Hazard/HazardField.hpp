#pragma once

#include "stdafx.hpp"
#include "Timer.hpp"
#include "IOComponent.hpp"

class Universe;

class HazardField
{

public:
	HazardField(Universe* universe, const Json::Value& root);

	void spawn();

	void input(std::string command, sf::Packet data);
private:
	Universe* m_pUniverse;
	std::string m_hazardName;
	int m_numHazards;
	float m_radius;
	b2Vec2 m_origin;

	IOComponent m_io;
};