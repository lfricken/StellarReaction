#include "DecorationEngine.hpp"

#include "QuadComponent.hpp"
#include "Decoration.hpp"
#include "Camera.hpp"
#include "Convert.hpp"

DecorationEngine::DecorationEngine() : m_deltaTime(0)
{

}
DecorationEngine::~DecorationEngine()
{

}
void DecorationEngine::update(const Vec2& cameraPos, const Vec2& halfSize)
{
	Vec2 bottomLeft = cameraPos - halfSize;
	Vec2 topRight = cameraPos + halfSize;

	float dTime = m_deltaTime.getTimeElapsed();;
	for(auto it = m_static.begin(); it != m_static.end(); ++it)
		(**it).updateScaledPosition(cameraPos, bottomLeft, topRight, dTime);
}
void DecorationEngine::loadJson(const Json::Value& root)
{
	m_deltaTime.getTimeElapsed();

	Decoration* pDecor = NULL;
	QuadComponent* pQuad = NULL;
	QuadComponentData quadData;
	DecorationData decorData;



	quadData.layer = GraphicsLayer::BackgroundClose;
	decorData.movementScale = 0.8f;
	quadData.dimensions.x = 8000;
	quadData.dimensions.y = 8000;
	decorData.realPosition = Vec2(0, 0);
	quadData.texName = "backgrounds/stardust1.png";
	quadData.animSheetName = "backgrounds/stardust1.acfg";


	pQuad = new QuadComponent(quadData);
	pDecor = new Decoration(decorData, pQuad);
	m_static.push_back(sptr<Decoration>(pDecor));







	quadData.layer = GraphicsLayer::Effects;
	decorData.movementScale = -0.5f;
	quadData.dimensions.x = 16000;
	quadData.dimensions.y = 16000;
	decorData.realPosition = Vec2(0, 0);
	quadData.texName = "backgrounds/stardust1.png";
	quadData.animSheetName = "backgrounds/stardust1.acfg";

	
	pQuad = new QuadComponent(quadData);
	pDecor = new Decoration(decorData, pQuad);
	m_static.push_back(sptr<Decoration>(pDecor));
}


