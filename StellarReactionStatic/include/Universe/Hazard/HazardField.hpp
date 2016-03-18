#pragma once

#include "stdafx.hpp"
#include "Timer.hpp"

class Universe;

class HazardField {

public:
	HazardField(Universe* universe, b2Vec2 origin);

	void update();
private:

	Timer timer;
	b2Vec2 origin;
	Universe* universe;
	bool spawned;
};