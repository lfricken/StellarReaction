#pragma once
#include "stdafx.hpp"
#include "Timer.hpp"

/// <summary>
/// Creates a shake equation and returns positions
/// </summary>
class Shaker
{
public:
	Shaker();
	~Shaker();

	/// <summary>
	/// Compute position of the camera at this time into the shake.
	/// </summary>
	Vec2 getPosition() const;
	/// <summary>
	/// Create the samples for the shake
	/// </summary>
	void generate(float duration, float frequency, float amplitude);



private:
	/// <summary>
	/// generates 1 sample
	/// </summary>
	Vec2 generateSample(float maxAmplitude) const;

	List<Vec2> m_samples;
	/// <summary>
	/// tells you where the shake should be
	/// </summary>
	Timer m_shakeTimer;
};

