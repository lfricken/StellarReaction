#include "DecorationEngine.hpp"
#include "QuadComponent.hpp"
#include "Decoration.hpp"
#include "Spinner.hpp"
#include "Camera.hpp"
#include "Convert.hpp"
#include "Player.hpp"
#include "JSON.hpp"
#include "BlueprintLoader.hpp"
#include "Particles.hpp"


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
	const Vec2 bottomLeft = cameraPos - halfSize;
	const Vec2 topRight = cameraPos + halfSize;
	const float dTime = m_deltaTime.getTimeElapsed();
	const float currentTime = game.getUniverse().getTime();

	{

		auto lastIt = m_fullParticles.end();
		for(auto it = m_fullParticles.begin(); it != m_fullParticles.end(); ++it)
		{
			if(it->first.first <= currentTime)//this should start fading
			{
				lastIt = it;//remember the last one we did
				m_fadingParticles[it->first.second] = it->second;//copy to fading list
				it->second->startFade(it->first.second - currentTime);
			}
			else
				(it->second)->updateScaledPosition(cameraPos, bottomLeft, topRight, zoom, dTime);
		}

		if(lastIt != m_fullParticles.end())//remove from front
			m_fullParticles.erase(m_fullParticles.begin(), ++lastIt);

	}
	{

		auto lastIt = m_fadingParticles.end();
		for(auto it = m_fadingParticles.begin(); it != m_fadingParticles.end(); ++it)
		{
			if(it->first <= currentTime)//this should expire
				lastIt = it;//find the last one to expire
			else
				(it->second)->updateScaledPosition(cameraPos, bottomLeft, topRight, zoom, dTime);
		}

		if(lastIt != m_fadingParticles.end())//remove from front
			m_fadingParticles.erase(m_fadingParticles.begin(), ++lastIt);

	}
	{
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
void DecorationEngine::spawnParticles(const String& particleBP, const Vec2& pos, Vec2 dir)
{
	if(dir.len() == 0)
		dir = Vec2(1, 1);
	dir = dir.unit();


	const Particles& effect = *game.getUniverse().getBlueprints().getParticleSPtr(particleBP);
	Decoration* pDecor = NULL;
	QuadComponent* pQuad = NULL;

	DecorationData decorData;
	Pair<float, float> expiration;
	decorData.spawnRandom = false;
	decorData.infiniteZ = false;
	decorData.repeats = false;
	decorData.realPosition = pos;
	decorData.zPos = 0;

	float time = game.getUniverse().getTime();

	for(int i = 0; i < effect.number; ++i)
	{
		dir = dir.rotate(Rand::get(-effect.randRadArc, effect.randRadArc));
		float vel = Rand::get(1.f, effect.randVelScalarMax) * effect.velocity;
		dir *= vel;

		decorData.maxVelocity = dir;
		decorData.minVelocity = dir;

		decorData.maxSpinRate = Math::toDeg(effect.maxSpinRate);
		decorData.minSpinRate = Math::toDeg(effect.minSpinRate);

		pQuad = new QuadComponent(effect.quadData);
		pDecor = new Decoration(decorData, pQuad);
		expiration.first = time + effect.duration + 0.001*i;
		expiration.second = expiration.first + effect.fadeTime;
		m_fullParticles[expiration] = sptr<Decoration>(pDecor);
	}
}
