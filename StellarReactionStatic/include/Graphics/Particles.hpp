#pragma once

#include "stdafx.hpp"
#include "BlueprintData.hpp"
#include "QuadComponent.hpp"
#include "ClassRegister.hpp"

struct Particles : public BlueprintData
{
public:
	Particles()
	{
		duration = 1;
		fadeTime = 1;
		number = 1;

		velocity = 2;
		randArc = 45.f;

		randVelScalarMax = 1.1f;
		initialOrientation = 0.f;

		minSpinRate = 90.f;
		maxSpinRate = 360.f;


	}
	///How long is the particle in full state.
	float duration;
	///How long is the particle in fading state.
	float fadeTime;
	int number;

	///Arc of trajectory in Deg
	float randArc;
	///Initial velocity 
	float velocity;

	///Choose between this and 1 as velocity multiplier.
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

	MyType(Particles, Particles);
};