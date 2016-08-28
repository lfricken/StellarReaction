#include "DecorationEngine.hpp"
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
	const float dTime = m_deltaTime.getTimeElapsed();

	for(auto it = m_particleSets.begin(); it != m_particleSets.end(); ++it)
	{
		it->timeRemain -= dTime;
		if(dTime <= 0)
		{
			for(auto decorIt = m_decorations.begin(); decorIt != m_decorations.end(); ++decorIt)
				if(decorIt->get() == it->beginSet)
				{
					if(!it->startedFade)
						(**decorIt).startFade(it->fadeTimeRemain);
					it->fadeTimeRemain -= dTime;
				}
			it->startedFade = true;
		}
	}
	{
		Vec2 bottomLeft = cameraPos - halfSize;
		Vec2 topRight = cameraPos + halfSize;


		for(auto it = m_decorations.begin(); it != m_decorations.end(); ++it)
			(**it).updateScaledPosition(cameraPos, bottomLeft, topRight, zoom, dTime);
	}
}
void DecorationEngine::loadJson(const Json::Value& decorations)
{
	m_deltaTime.getTimeElapsed();

	const float maxZoom = game.getLocalPlayer().getCamera().m_maxZoom;
	const float maxDim = (float)Math::max(game.getWindow().getSize().x / 2, game.getWindow().getSize().y / 2) / scale;
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
			float x = Rand::get(-maxHalf.x, maxHalf.x);
			float y = Rand::get(-maxHalf.y, maxHalf.y);
			(**it).setPosition(cameraPos + Vec2(x,y));
		}
	}
}
void DecorationEngine::spawnParticles(const Particles& effect)
{
	Decoration* pDecor = NULL;
	QuadComponent* pQuad = NULL;
	QuadComponentData quadData;
	DecorationData decorData;

	for(int i = 0; i < effect.number; ++i)
	{
		float arc = Rand::get(-effect.radArc, effect.radArc);
		float spin = Rand::get(-effect.randSpin, effect.randSpin);
		float velScalar = Rand::get(1.f, effect.randVelScalarMax);
		Vec2 vel(effect.velocity.x*velScalar, effect.velocity.y*velScalar);

		pQuad = new QuadComponent(quadData);
		pDecor = new Decoration(decorData, pQuad);
		m_decorations.push_back(sptr<Decoration>(pDecor));
	}
}
