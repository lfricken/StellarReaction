#pragma once
#include "stdafx.hpp"

/// <summary>
/// 
/// </summary>
class BuildBounds
{
public:
	BuildBounds();
	~BuildBounds();

	bool contains(const Vec2& point);

	Vec2 center;
	float radius;
};

