#include "BlackHoleGraphic.hpp"

using namespace std;

void BlackHoleGraphicData::loadJson(const Json::Value& root)
{
	LOADJSON(innerDecor);
	LOADJSON(middleDecor);
	LOADJSON(outerDecor);

	ModuleData::loadJson(root);
}

BlackHoleGraphic::BlackHoleGraphic(const BlackHoleGraphicData& rData) : Module(rData)
{
	m_decors.push_back(sptr<GraphicsComponent>(new Spinner(rData.innerDecor)));
	m_decors.push_back(sptr<GraphicsComponent>(new Spinner(rData.middleDecor)));
	m_decors.push_back(sptr<GraphicsComponent>(new Spinner(rData.outerDecor)));
	b2Vec2 parentPos = m_parentChunk->getBodyPtr()->GetPosition();
	m_decors[0]->setPosition(parentPos);
	m_decors[1]->setPosition(parentPos);
	m_decors[2]->setPosition(parentPos);
}
BlackHoleGraphic::~BlackHoleGraphic()
{

}
void BlackHoleGraphic::prePhysUpdate()
{

}
