#include "BlackHole.hpp"

using namespace std;

void BlackHoleData::loadJson(const Json::Value& root)
{
	LOADJSON(innerDecor);
	LOADJSON(middleDecor);
	LOADJSON(outerDecor);

	ModuleData::loadJson(root);
}

BlackHole::BlackHole(const BlackHoleData& rData) : Module(rData)
{
	m_decors.push_back(sptr<GraphicsComponent>(new Spinner(rData.innerDecor)));
	m_decors.push_back(sptr<GraphicsComponent>(new Spinner(rData.middleDecor)));
	m_decors.push_back(sptr<GraphicsComponent>(new Spinner(rData.outerDecor)));
	b2Vec2 parentPos = m_parentChunk->getBodyPtr()->GetPosition();
	m_decors[0]->setPosition(parentPos);
	m_decors[1]->setPosition(parentPos);
	m_decors[2]->setPosition(parentPos);
}
BlackHole::~BlackHole()
{

}
void BlackHole::prePhysUpdate()
{

}
