#pragma once

#include "stdafx.hpp"
#include "Timer.hpp"

class Camera;
class Decoration;

/// Manages Decorations, probably this code should just be in Universe.
class DecorationEngine
{
public:
	DecorationEngine();
	~DecorationEngine();
	
	/// 
	void update(const Vec2& cameraPos, const Vec2& halfSize, const float zoom);

	void loadJson(const Json::Value& root);

	void initSpawns(const Vec2& cameraPos, const Vec2& maxHalf);

private:
	vector<sptr<Decoration> > m_decorations;

	Timer m_deltaTime;
};

