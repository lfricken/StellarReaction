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

/// <summary>
/// Something conveinent for using as a meter (for gui)
/// </summary>
class LinearMeter : public QuadComponent
{
public:
	LinearMeter(const LinearMeterData& rData);
	virtual ~LinearMeter();

	void postUpdate();//used to override texCoords
	
	/// <summary>
	/// as percent approaches 1, the initial y read position of the texture increases to 1*height
	/// this means at 1, the texture is being read 1 tile below the start tile
	/// </summary>
	/// <param name="per">The per.</param>
	void setPercent(float per);// 0-1

protected:
private:

	float m_percent;//how full are we (0-1)
};

#endif // LINEARMETER_HPP
