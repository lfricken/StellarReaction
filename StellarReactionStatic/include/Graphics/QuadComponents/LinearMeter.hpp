#pragma once

#include "QuadComponent.hpp"

/// Data needed for LinearMeter
struct LinearMeterData : public QuadComponentData
{
	LinearMeterData() :
		QuadComponentData()
	{
		texName = "fill/white.png";
	}

};


/// Something convenient for using as a meter (for gui).
class LinearMeter : public QuadComponent
{
public:
	LinearMeter(const LinearMeterData& rData);
	virtual ~LinearMeter();

	void postUpdate();//used to override texCoords
	
	/// Determines where on the texture to read.
	/// At 0, it will read the default texture tile.
	/// As percent goes to 1, it moves the tile being read to the tile below.
	void setPercent(float per);// 0-1

protected:
private:

	float m_percent;
};

