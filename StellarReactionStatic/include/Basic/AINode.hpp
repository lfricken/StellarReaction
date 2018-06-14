#pragma once

#include "stdafx.hpp"

class AINode
{
public:
	AINode();
	~AINode();

	const Vec2& getPosition() const;
	void setPosition(const Vec2& newPos);

private:


	Vec2 m_position;
};

