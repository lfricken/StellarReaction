#pragma once

#include "stdafx.hpp"
#include "Blueprintable.hpp"
#include "QuadComponent.hpp"
#include "ClassRegister.hpp"

struct Particles : public BlueprintableData
{
public:
	Particles()
	{
		randDuration = 1;
		duration = 1;
		fadeInTime = 1;
		fadeOutTime = 1;
		number = 1;

		velocity = 2;
		randArc = 45.f;

		randVelScalarMax = 1.1f;
		initialOrientation = 0.f;

		minSpinRate = 90.f;
		maxSpinRate = 360.f;


	}
	/// <summary>
	/// choose a time between 0 and value to add to the duration
	/// </summary>
	float randDuration;
	/// <summary>
	/// How long is the particle in full state.
	/// </summary>
	float duration;
	/// <summary>
	/// How long is the particle fading in.
	/// </summary>
	float fadeInTime;
	/// <summary>
	/// How long is the particle fading out.
	/// </summary>
	float fadeOutTime;
	/// <summary>
	/// How many particles total.
	/// </summary>
	int number;

	///Bounding arc of trajectory in Degrees
	float randArc;
	///Initial velocity 
	float velocity;

	///Choose between this value and 1 as velocity multiplier.
	float randVelScalarMax;
	///Initial Orientation in rad CCW
	float initialOrientation;

	///Min deg CCW ps 
	float minSpinRate;
	///Max deg CCW ps 
	float maxSpinRate;

	QuadComponentData quadData;

	virtual void loadJson(const Json::Value& root);
	virtual Particles* clone() const;
};