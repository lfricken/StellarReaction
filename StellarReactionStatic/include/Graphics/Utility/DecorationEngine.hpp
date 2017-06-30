#pragma once

#include "stdafx.hpp"
#include "Timer.hpp"


class Camera;
class Decoration;
struct DecorationData;
struct Particles;

/// Manages Decorations, probably this code should just be in Universe.
class DecorationEngine
{
public:
	DecorationEngine();
	~DecorationEngine();
	
	/// Update the positions the Decorations related to 
	void update(const Vec2& cameraPos, const Vec2& halfSize, const float zoom);
	/// Load decorations from some JSON object.
	void loadJson(const Json::Value& root);
	/// Background objects need to spawn randomly with respect to the
	/// local player, which can only happen after a certain point.
	/// So we call this after the player is created.
	void initSpawns(const Vec2& cameraPos, const Vec2& maxHalf);
	/// Creates particle effects.
	void spawnParticles(const String& particleBP, const Vec2& pos, const Vec2& dir, const Vec2& transverse);
private:
	void spawnParticle(DecorationData decorData, const Particles& effect, const Vec2& dir, const Vec2& transverse, float time, int i);

	/// <summary>
	/// List of all decorations.
	/// </summary>
	List<sptr<Decoration> > m_decorations;
	/// <summary>
	/// List of particles (fade time, expire time) that havent started to fade.
	/// </summary>
	Map<Pair<float,float>, sptr<Decoration> > m_fullParticles;
	/// List of particles <expire time> that have started to fade.
	Map<float, sptr<Decoration> > m_fadingParticles;
	/// Used to determine how much time has passed.
	Timer m_deltaTime;
};

