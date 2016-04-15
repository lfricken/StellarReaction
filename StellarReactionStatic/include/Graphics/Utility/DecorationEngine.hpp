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

	void update(const Vec2& cameraPos, const Vec2& halfSize);

	void loadJson(const Json::Value& root);

private:
	vector<sptr<Decoration> > m_static;
	vector<sptr<Decoration> > m_dynamic;

	Timer m_deltaTime;
};

