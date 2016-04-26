#pragma once

#include "QuadComponent.hpp"

/// Used to initialize a Spinner.
struct SpinnerData : public QuadComponentData
{
	SpinnerData() :
		rate(90),//degrees CCW/s
		randomRot(true),
		startOn(true)
	{

	}
	float rate;//degrees CCW/s
	bool randomRot;//will we spawn with a random initial rotation?
	bool startOn;//starts rotating

	virtual void loadJson(const Json::Value& root);
};


/// Used for graphics objects that should rotate steadily.
class Spinner : public QuadComponent
{
public:
	Spinner(const SpinnerData& rData);
	virtual ~Spinner();

	/// Set whether the Spinner is spinning or not.
	void toggleOn(bool on);
	/// Sets the rotation rate for this Spinner in degrees counterclockwise per second.
	void setRotationRate(float degCCW);
	/// Called to update how this object is displayed.
	virtual void postUpdate();

protected:
private:

	/// Is this Spinner spinning?
	bool m_on;
	/// Radians CCW per second.
	float m_rate;
	/// Used to determine how much time has passed.
	Timer m_timer;
};

