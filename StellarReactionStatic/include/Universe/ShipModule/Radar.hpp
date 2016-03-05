#ifndef RADAR_HPP
#define RADAR_HPP

#include "ShipModule.hpp"
#include "Spinner.hpp"

struct RadarData;

class Radar : public ShipModule
{
public:
	Radar(const RadarData& rData);
	virtual ~Radar();

	virtual void setHealthStateHook(HealthState newState);
	virtual void toggleStealth(bool toggle);

protected:
private:
	bool m_hasContributed;//
	float m_zoom;//zoom change

	int m_dishIndex;//the index of our dish in our list
};


struct RadarData : public ShipModuleData
{
	RadarData() :
		ShipModuleData(),
		zoomAddition(1)///units?
	{
		baseDecor.texName = "radar/radar_base.png";
		baseDecor.animSheetName = "radar/radar_base.acfg";

		dish.texName = "radar/dish.png";
		dish.animSheetName = "radar/dish.acfg";
		dish.randomRot = true;
		dish.rate = 90;//deg/s
		dish.layer = GraphicsLayer::ShipAppendagesLower;
	}

	SpinnerData dish;

	float zoomAddition;//how much more we can zoom with this module active

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		RadarData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Radar(copy);
	}
	virtual ModuleData* clone() const
	{
		return new RadarData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, RadarData);
};



#endif // RADAR_HPP
