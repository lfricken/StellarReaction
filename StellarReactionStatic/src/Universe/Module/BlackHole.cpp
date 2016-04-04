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
	m_decors.push_back(sptr<GraphicsComponent>(new QuadComponent(rData.innerDecor)));
	m_decors.push_back(sptr<GraphicsComponent>(new QuadComponent(rData.middleDecor)));
	m_decors.push_back(sptr<GraphicsComponent>(new QuadComponent(rData.outerDecor)));
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
void BlackHole::postPhysUpdate()
{
	m_decors[0]->setRotation(m_decors[0]->getRotation() - 0.01f);
	m_decors[1]->setRotation(m_decors[1]->getRotation() - 0.005f);
	m_decors[2]->setRotation(m_decors[2]->getRotation() - 0.002f);
}
