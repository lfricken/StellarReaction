#pragma once

#include "stdafx.hpp"
#include "Timer.hpp"
#include "QuadComponent.hpp"

class Camera;
class Decoration;

/// Manages Decorations, probably this code should just be in Universe.
class DecorationEngine
{
public:
	struct Particles
	{
		Particles()
		{
			duration = 1;
			fadeTime = 1;
			number = 1;

			spawn = Vec2(0, 0);
			velocity = Vec2(1, 1);
			randRadArc = Math::toRad(45.f);

			randVelScalarMax = 1.1f;
			initialOrientation = Math::toRad(0.f);

			minSpinRate = Math::toRad(200.f);
			maxSpinRate = Math::toRad(200.f);
		}
		float duration;
		float fadeTime;
		int number;

		Vec2 spawn;
		float randRadArc;
		///Initial velocity 
		Vec2 velocity;

		///Choose between this and 1 as velocity multiplier.
		float randVelScalarMax;
		///Initial Orientation in rad CCW
		float initialOrientation;

		///Min radians CCW ps 
		float minSpinRate;
		///Max radians CCW ps 
		float maxSpinRate;

		QuadComponentData quadData;
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


	/// List of all decorations.
	vector<sptr<Decoration> > m_decorations;
	/// List of particles <fade time, expire time> that haven't started to fade.
	Map<Pair<float,float>, sptr<Decoration> > m_fullParticles;
	/// List of particles <expire time> that have started to fade.
	Map<float, sptr<Decoration> > m_fadingParticles;
	/// Used to determine how much time has passed.
	Timer m_deltaTime;
};

