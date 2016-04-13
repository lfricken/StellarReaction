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

	void update(const Camera& camera);

	void loadJson(const Json::Value& root);

private:
	vector<sptr<Decoration> > m_static;
	vector<sptr<Decoration> > m_dynamic;

	Timer m_deltaTime;
};

