#pragma once

#include "stdafx.hpp"

#include "Timer.hpp"

class Camera;
class Decoration;

class DecorationEngine
{
public:
	DecorationEngine();
	~DecorationEngine();
	
	void update(const Vec2& cameraPos, const Vec2& halfSize, const float zoom);

	void loadJson(const Json::Value& root);

private:
	//void tile(const DecorationData& decor, const QuadComponentData& quad);//tile these

	vector<sptr<Decoration> > m_static;
	vector<sptr<Decoration> > m_dynamic;

	Timer m_deltaTime;
};

