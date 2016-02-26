#include "QuadComponent.hpp"

#include "Convert.hpp"
#include "BatchLayers.hpp"
#include "Globals.hpp"
#include "Universe.hpp"

using namespace std;
using namespace sf;

QuadComponent::QuadComponent(const QuadComponentData& rData) : GraphicsComponent(rData)
{
	m_numVerts = 4;

	m_originPos.resize(m_numVerts);
	m_originPos[0] = sf::Vector2f(-rData.dimensions.x/2, -rData.dimensions.y/2);
	m_originPos[1] = sf::Vector2f(-rData.dimensions.x/2,  rData.dimensions.y/2);
	m_originPos[2] = sf::Vector2f( rData.dimensions.x/2,  rData.dimensions.y/2);
	m_originPos[3] = sf::Vector2f( rData.dimensions.x/2, -rData.dimensions.y/2);

	m_originTex.resize(m_numVerts);
	m_originTex[0] = sf::Vector2f(0, 0);
	m_originTex[1] = sf::Vector2f(0, 1);
	m_originTex[2] = sf::Vector2f(1, 1);
	m_originTex[3] = sf::Vector2f(1, 0);


	QuadData dat = game.getUniverse().getBatchLayers().request(rData.texName, rData.layer);

	m_pVerts = dat.vertexList;
	m_startVert = dat.firstElement;

	setColor(rData.color);
}
QuadComponent::~QuadComponent()
{

}
void QuadComponent::postUpdate()
{
	
}
void QuadComponentData::loadJson(const Json::Value& root)
{
	if(!root["dimensions"].isNull())
	{
		dimensions.x = root["dimensions"][0].asInt();
		dimensions.y = root["dimensions"][1].asInt();
	}

	if(!root["permanentRot"].isNull())
		permanentRot = root["permanentRot"].asFloat();

	if(!root["center"].isNull())
	{
		center.x = root["center"][0].asInt();
		center.y = root["center"][1].asInt();
	}

	if(!root["texName"].isNull())
		texName = root["texName"].asString();
	if(!root["animSheetName"].isNull())
		animSheetName = root["animSheetName"].asString();
	if(!root["layer"].isNull())
		layer = ChooseLayer(root["layer"].asString());
}

