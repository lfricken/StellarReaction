#ifndef SPINNER_HPP
#define SPINNER_HPP

#include "QuadComponent.hpp"

struct SpinnerData : public QuadComponentData
{
	SpinnerData() :
		rate(90),//degrees CCW/s
		randomRot(true),
		startOn(true)
	{

	}
	float rate;//degrees CCW/s
	bool randomRot;//will we spawn with a random rotation?
	bool startOn;
};


class Spinner : public QuadComponent
{
public:
	Spinner(const SpinnerData& rData);
	virtual ~Spinner();

	void toggleOn(bool on);//send true to make it spin, defaults on
	void setRotationRate(float degCCW);//degrees CCW per second
	virtual void postUpdate();

protected:
private:

	bool m_on;
	float m_oldRate;
	float m_rate;// radiansCCW/s
	Timer m_timer;//tells us how long since last update
};

#endif // SPINNER_HPP
