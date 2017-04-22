#pragma once

#include "QuadComponent.hpp"

/// Data needed for LinearMeter
struct LinearMeterData : public QuadComponentData
{
	LinearMeterData() :
		QuadComponentData()
	{
		texName = "fill/white";
	}

};


/// Something convenient for using as a meter (for gui).
class LinearMeter : public QuadComponent
{
public:
	LinearMeter(const LinearMeterData& rData);
	virtual ~LinearMeter();
	
	/// \brief Set the meter full percentage.
	/// 
	/// Determines where on the texture to read.
	/// At 0, it will read the default texture tile it normally would.
	/// As percent goes to 1, it moves the tile being read to the tile below.
	void setPercent(float per);

	/// Called to update how this object is displayed.
	virtual void postUpdate();
protected:
private:
	/// Percent [0-1] that this meter is set to.
	float m_percent;
};

