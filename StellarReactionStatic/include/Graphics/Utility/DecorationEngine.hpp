#pragma once

#include "stdafx.hpp"

class Camera;
class Decoration;

class DecorationEngine
{
public:
	DecorationEngine();
	~DecorationEngine();

	void update(const Camera& cameraPos);

	void loadJson(const Json::Value& root);

private:
	vector<sptr<Decoration> > m_static;
	vector<sptr<Decoration> > m_dynamic;
};

