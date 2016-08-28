#pragma once

#include "stdafx.hpp"
#include "Timer.hpp"

class Camera;
class Decoration;

/// Manages Decorations, probably this code should just be in Universe.
class DecorationEngine
{
public:
	struct Particles
	{
		float duration;
		float fadeTime;
		int number;
		Vec2 spawn;
		float radArc;
		Vec2 velocity;
		float randVelScalarMax;
		float randSpin;
	};

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
	void spawnParticles(const Particles& effect);
private:

	struct ParticleSet
	{
		ParticleSet()
		{
			startedFade = false;
		}
		Decoration* beginSet;
		int numParticles;
		float timeRemain;
		float fadeTimeRemain;
		bool startedFade;
	};
	/// List of all decorations.
	vector<sptr<Decoration> > m_decorations;
	vector<ParticleSet> m_particleSets;
	/// Used to determine how much time has passed.
	Timer m_deltaTime;
};

