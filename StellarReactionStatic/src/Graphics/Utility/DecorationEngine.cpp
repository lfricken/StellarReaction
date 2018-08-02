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


DecorationEngine::DecorationEngine() : m_deltaTime(0.f)
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
	const float currentTime = getGame()->getUniverse().getTime();

	{//particles fading in or fading

		auto lastIt = m_fullParticles.end();
		for(auto it = m_fullParticles.begin(); it != m_fullParticles.end(); ++it)
		{
			if(it->first.first <= currentTime)//this should start fading
			{
				lastIt = it;//remember the last one we did
				m_fadingParticles[it->first.second] = it->second;//copy to fading list
				it->second->startFadeOut(it->first.second - currentTime);
			}
			else
				(it->second)->updateScaledPosition(cameraPos, bottomLeft, topRight, zoom, dTime);
		}

		if(lastIt != m_fullParticles.end())//remove from front
			m_fullParticles.erase(m_fullParticles.begin(), ++lastIt);

	}
	{//particles fading

		auto lastIt = m_fadingParticles.end();
		//they are ordered from first to expire to last to expire
		for(auto it = m_fadingParticles.begin(); it != m_fadingParticles.end(); ++it)
		{
			if(it->first <= currentTime)//this should expire
				lastIt = it;//find the last one to expire
			else
				(it->second)->updateScaledPosition(cameraPos, bottomLeft, topRight, zoom, dTime);
		}
		//lastIt = end-1 if all particles should be removed, so increment by 1 since erase(inclusive, exclusive)
		if(lastIt != m_fadingParticles.end())//remove from front to back
			m_fadingParticles.erase(m_fadingParticles.begin(), ++lastIt);

	}
	{//normal decorations
		for(auto it = m_decorations.begin(); it != m_decorations.end(); ++it)
			(**it).updateScaledPosition(cameraPos, bottomLeft, topRight, zoom, dTime);
	}
}
void DecorationEngine::loadJson(const Json::Value& decorations)
{
	m_deltaTime.getTimeElapsed();

	//const float maxZoom = getGame()->getLocalPlayer().getCamera().m_maxZoom;
	//const float maxDim = (float)Math::max(getGame()->getWindow().getSize().x / 2, getGame()->getWindow().getSize().y / 2) / scale;
	//const float halfSize = maxZoom * maxDim;


	for(auto it = decorations.begin(); it != decorations.end(); ++it)
	{
		auto root = *it;
		int count = 1;
		Decoration* pDecor = nullptr;
		QuadComponent* pQuad = nullptr;
		QuadComponentData quadData;
		DecorationData decorData;

		GETJSON(count);
		LOADJSON(quadData);
		LOADJSON(decorData);

		for(int i = 0; i < count; ++i)
		{
			pQuad = new QuadComponent(quadData);
			pDecor = new Decoration(decorData, sptr<GraphicsComponent>(pQuad));
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
void DecorationEngine::spawnParticles(const String& particleBP, const Vec2& pos, const Vec2& dir, const Vec2& transverse)
{
	auto direction = dir;
	if(direction.len() == 0)
		direction = Vec2(1, 1);
	direction = direction.unit();

	const Particles& effect = *getGame()->getUniverse().getBlueprints().getParticleSPtr(particleBP);

	DecorationData decorData;
	Pair<float, float> expiration;
	decorData.spawnRandom = false;
	decorData.infiniteZ = false;
	decorData.repeats = false;
	decorData.realPosition = pos;
	decorData.zPos = 0;

	float time = getGame()->getUniverse().getTime();

	assert(!isinf(direction.x) && !isinf(direction.y));

	for(int i = 0; i < effect.number; ++i)
		spawnParticle(decorData, effect, direction, transverse, time, i);

}
void DecorationEngine::spawnParticle(DecorationData decorData, const Particles& effectConst, const Vec2& dir, const Vec2& transverse, float time, int i)
{
	Pair<float, float> expiration;
	Particles effect = effectConst;

	Vec2 randDir = dir;
	auto rot = Rand::get(-effect.randArc, effect.randArc);
	randDir = dir.rotate(Math::toRad(rot));

	float vel = Rand::get(1.f, effect.randVelScalarMax) * effect.velocity;
	randDir *= vel;

	decorData.maxSpinRate = (effect.maxSpinRate);
	decorData.minSpinRate = (effect.minSpinRate);

	effect.quadData.permanentRot = Math::toDeg(randDir.toAngle());

	randDir += transverse;
	decorData.maxVelocity = randDir;
	decorData.minVelocity = randDir;

	auto pQuad = new QuadComponent(effect.quadData);
	auto pDecor = new Decoration(decorData, sptr<GraphicsComponent>(pQuad));
	float randDuration = Rand::get(0.f, effect.randDuration);
	expiration.first = time + effect.duration + randDuration + effect.fadeInTime + 0.001f*i;//adding that strange value so it can be added to the map without overlap
	expiration.second = expiration.first + effect.fadeOutTime;
	m_fullParticles[expiration] = sptr<Decoration>(pDecor);
	pDecor->startFadeIn(effect.fadeInTime);
}