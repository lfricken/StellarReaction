#ifndef LINEARMETER_HPP
#define LINEARMETER_HPP

#include "QuadComponent.hpp"

struct LinearMeterData : public QuadComponentData
{
	LinearMeterData() :
		QuadComponentData()
	{
		texName = "fill/white.png";
	}

};


class LinearMeter : public QuadComponent
{
public:
	LinearMeter(const LinearMeterData& rData);
	virtual ~LinearMeter();

	void postUpdate();//used to override texCoords

	void setPercent(float per);// 0-1

protected:
private:

	float m_percent;//how full are we (0-1)
};

#endif // LINEARMETER_HPP
