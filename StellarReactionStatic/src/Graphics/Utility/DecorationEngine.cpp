#include "DecorationEngine.hpp"

#include "QuadComponent.hpp"
#include "Decoration.hpp"
#include "Spinner.hpp"
#include "Camera.hpp"
#include "Convert.hpp"
#include "Player.hpp"

DecorationEngine::DecorationEngine() : m_deltaTime(0)
{

}
DecorationEngine::~DecorationEngine()
{

}
/// <summary>
/// Update positions of decorations.
/// Potentially spawn more (from the dynamic)
/// </summary>
/// <param name="cameraPos">The camera position.</param>
/// <param name="halfSize">Size of the half.</param>
void DecorationEngine::update(const Vec2& cameraPos, const Vec2& halfSize, const float zoom)
{
	Vec2 bottomLeft = cameraPos - halfSize;
	Vec2 topRight = cameraPos + halfSize;

	float dTime = m_deltaTime.getTimeElapsed();;
	for(auto it = m_static.begin(); it != m_static.end(); ++it)
		(**it).updateScaledPosition(cameraPos, bottomLeft, topRight, zoom, dTime);
}
void DecorationEngine::loadJson(const Json::Value& root)
{
	m_deltaTime.getTimeElapsed();

	const float maxZoom = game.getLocalPlayer().getCamera().m_maxZoom;
	const float maxDim = std::max(game.getWindow().getSize().x / 2, game.getWindow().getSize().y / 2) / scale;
	const float halfSize = maxZoom * maxDim;

	//{
	//	Decoration* pDecor = NULL;
	//	QuadComponent* pQuad = NULL;
	//	QuadComponentData quadData;
	//	DecorationData decorData;

	//	quadData.layer = GraphicsLayer::BackgroundVeryBack;
	//	quadData.dimensions.x = 512;
	//	quadData.dimensions.y = 512;
	//	quadData.texName = "backgrounds/debris/debris1.png";
	//	quadData.animSheetName = "backgrounds/debris/debris.acfg";

	//	decorData.movementScale = 0.93f;
	//	decorData.sizeScale = 0.1f;

	//	decorData.repeatsRandom = true;
	//	decorData.positionRandom = true;
	//	decorData.maxSpinRate = 50;
	//	decorData.minSpinRate = 20;
	//	decorData.maxVelocity = Vec2(1, 1);
	//	decorData.minVelocity = Vec2(-1, -1);
	//	decorData.realPosition = Vec2(0, 0);

	//	pQuad = new QuadComponent(quadData);
	//	pDecor = new Decoration(decorData, pQuad);
	//	m_static.push_back(sptr<Decoration>(pDecor));
	//}
	//{
	//	Decoration* pDecor = NULL;
	//	QuadComponent* pQuad = NULL;
	//	QuadComponentData quadData;
	//	DecorationData decorData;

	//	quadData.layer = GraphicsLayer::BackgroundVeryBack;
	//	quadData.dimensions.x = 1024;
	//	quadData.dimensions.y = 1024;
	//	quadData.texName = "backgrounds/debris/debris10.png";
	//	quadData.animSheetName = "backgrounds/debris/debris.acfg";

	//	decorData.movementScale = 0.9f;
	//	decorData.sizeScale = 0.1f;

	//	decorData.repeatsRandom = true;
	//	decorData.positionRandom = true;
	//	decorData.maxSpinRate = 50;
	//	decorData.minSpinRate = 20;
	//	decorData.maxVelocity = Vec2(1, 1);
	//	decorData.minVelocity = Vec2(-1, -1);
	//	decorData.realPosition = Vec2(0, 0);

	//	pQuad = new QuadComponent(quadData);
	//	pDecor = new Decoration(decorData, pQuad);
	//	m_static.push_back(sptr<Decoration>(pDecor));
	//}
	//{
	//	Decoration* pDecor = NULL;
	//	QuadComponent* pQuad = NULL;
	//	QuadComponentData quadData;
	//	DecorationData decorData;

	//	quadData.layer = GraphicsLayer::BackgroundVeryFar;
	//	quadData.dimensions.x = 1536;
	//	quadData.dimensions.y = 1536;
	//	quadData.texName = "backgrounds/debris/debris1.png";
	//	quadData.animSheetName = "backgrounds/debris/debris.acfg";

	//	decorData.movementScale = 0.8f;
	//	decorData.sizeScale = 0.1f;

	//	decorData.repeatsRandom = true;
	//	decorData.positionRandom = true;
	//	decorData.maxSpinRate = 50;
	//	decorData.minSpinRate = 20;
	//	decorData.maxVelocity = Vec2(1, 1);
	//	decorData.minVelocity = Vec2(-1, -1);
	//	decorData.realPosition = Vec2(0, 0);

	//	pQuad = new QuadComponent(quadData);
	//	pDecor = new Decoration(decorData, pQuad);
	//	m_static.push_back(sptr<Decoration>(pDecor));
	//}
	{
		Decoration* pDecor = NULL;
		QuadComponent* pQuad = NULL;
		QuadComponentData quadData;
		DecorationData decorData;

		quadData.layer = GraphicsLayer::BackgroundFar;
		quadData.dimensions.x = 2048;
		quadData.dimensions.y = 2048;
		quadData.texName = "backgrounds/debris/debris1.png";
		quadData.animSheetName = "backgrounds/debris/debris.acfg";
		quadData.color = sf::Color(96, 96, 96);

		decorData.movementScale = 0.8f;
		decorData.sizeScale = 0.02f;

		decorData.repeatsRandom = true;
		decorData.positionRandom = true;
		decorData.maxSpinRate = 50;
		decorData.minSpinRate = 20;
		decorData.maxVelocity = Vec2(1, 1);
		decorData.minVelocity = Vec2(-1, -1);
		decorData.realPosition = Vec2(0, 0);

		pQuad = new QuadComponent(quadData);
		pDecor = new Decoration(decorData, pQuad);
		m_static.push_back(sptr<Decoration>(pDecor));
	}
	{
		Decoration* pDecor = NULL;
		QuadComponent* pQuad = NULL;
		QuadComponentData quadData;
		DecorationData decorData;

		quadData.layer = GraphicsLayer::BackgroundFar;
		quadData.dimensions.x = 1024;
		quadData.dimensions.y = 1024;
		quadData.texName = "backgrounds/debris/debris1.png";
		quadData.animSheetName = "backgrounds/debris/debris.acfg";

		decorData.movementScale = 0.0f;
		decorData.sizeScale = 0.0f;

		decorData.repeatsRandom = true;
		decorData.positionRandom = true;
		decorData.maxSpinRate = 50;
		decorData.minSpinRate = 20;
		decorData.maxVelocity = Vec2(1, 1);
		decorData.minVelocity = Vec2(-1, -1);
		decorData.realPosition = Vec2(0, 0);

		pQuad = new QuadComponent(quadData);
		pDecor = new Decoration(decorData, pQuad);
		m_static.push_back(sptr<Decoration>(pDecor));
	}

	//{
	//	Decoration* pDecor = NULL;
	//	QuadComponent* pQuad = NULL;
	//	QuadComponentData quadData;
	//	DecorationData decorData;

	//	quadData.layer = GraphicsLayer::BackgroundVeryBack;
	//	decorData.movementScale = 0.8f;
	//	decorData.repeats = true;
	//	quadData.dimensions.x = 8000;
	//	quadData.dimensions.y = 8000;
	//	decorData.realPosition = Vec2(0, 0);
	//	quadData.texName = "backgrounds/stardust1.png";
	//	quadData.animSheetName = "backgrounds/stardust1.acfg";


	//	pQuad = new QuadComponent(quadData);
	//	pDecor = new Decoration(decorData, pQuad);
	//	m_static.push_back(sptr<Decoration>(pDecor));
	//}





	//{
	//	Decoration* pDecor = NULL;
	//	QuadComponent* pQuad = NULL;
	//	QuadComponentData quadData;
	//	DecorationData decorData;

	//	quadData.layer = GraphicsLayer::Effects;
	//	decorData.movementScale = -0.5f;
	//	decorData.repeats = true;
	//	quadData.dimensions.x = 16000;
	//	quadData.dimensions.y = 16000;
	//	decorData.realPosition = Vec2(0, 0);
	//	quadData.texName = "backgrounds/stardust1.png";
	//	quadData.animSheetName = "backgrounds/stardust1.acfg";


	//	pQuad = new QuadComponent(quadData);
	//	pDecor = new Decoration(decorData, pQuad);
	//	m_static.push_back(sptr<Decoration>(pDecor));
	//}


	{
		Decoration* pDecor = NULL;
		QuadComponent* pQuad = NULL;
		QuadComponentData quadData;
		DecorationData decorData;


		quadData.layer = GraphicsLayer::BackgroundVeryBack;
		decorData.movementScale = 1.f;
		decorData.sizeScale = 0.90f;
		quadData.dimensions.x = 2000;
		quadData.dimensions.y = 2000;
		decorData.realPosition = Vec2(0, 0);
		quadData.texName = "backgrounds/bg5.png";
		quadData.animSheetName = "backgrounds/bg5.acfg";


		pQuad = new QuadComponent(quadData);
		pDecor = new Decoration(decorData, pQuad);
		m_static.push_back(sptr<Decoration>(pDecor));
	}
}


