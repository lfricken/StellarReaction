#include "DecorationEngine.hpp"
#include "Random.hpp"
#include "QuadComponent.hpp"
#include "Decoration.hpp"
#include "Spinner.hpp"
#include "Camera.hpp"
#include "Convert.hpp"
#include "Player.hpp"
#include "JSON.hpp"

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
	for(auto it = m_decorations.begin(); it != m_decorations.end(); ++it)
		(**it).updateScaledPosition(cameraPos, bottomLeft, topRight, zoom, dTime);
}
void DecorationEngine::loadJson(const Json::Value& decorations)
{
	m_deltaTime.getTimeElapsed();

	const float maxZoom = game.getLocalPlayer().getCamera().m_maxZoom;
	const float maxDim = std::max(game.getWindow().getSize().x / 2, game.getWindow().getSize().y / 2) / scale;
	const float halfSize = maxZoom * maxDim;


	for(auto it = decorations.begin(); it != decorations.end(); ++it)
	{
		auto root = *it;
		int count = 1;
		Decoration* pDecor = NULL;
		QuadComponent* pQuad = NULL;
		QuadComponentData quadData;
		DecorationData decorData;

		GETJSON(count);
		LOADJSON(quadData);
		LOADJSON(decorData);

		for(int i = 0; i < count; ++i)
		{
			pQuad = new QuadComponent(quadData);
			pDecor = new Decoration(decorData, pQuad);
			m_decorations.push_back(sptr<Decoration>(pDecor));
		}
	}
}
void DecorationEngine::initSpawns(const Vec2& cameraPos, const Vec2& maxHalf)
{
	for(auto it = m_decorations.begin(); it != m_decorations.end(); ++it)
	{
		if((**it).isRandSpawn())
		{
			(**it).m_lastCameraPos = cameraPos;
			float x = Random::get(-maxHalf.x, maxHalf.x);
			float y = Random::get(-maxHalf.y, maxHalf.y);
			(**it).setPosition(cameraPos + Vec2(x,y));
		}
	}
}

