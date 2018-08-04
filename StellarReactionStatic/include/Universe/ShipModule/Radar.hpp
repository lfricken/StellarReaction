#ifndef RADAR_HPP
#define RADAR_HPP

#include "ShipModule.hpp"
#include "Spinner.hpp"

struct RadarData;


/// Adds to the Radar Pool, allowing the player to zoom out further.
class Radar : public ShipModule
{
public:
	Radar(const RadarData& rData);
	virtual ~Radar();
	///Actions to process on object when HealthState changes.
	virtual void setHealthStateHook(HealthState newState);
	///Toggle stealth state.
	virtual void toggleStealth(bool toggle);

protected:
private:

	///The index of our dish in our list.
	int m_dishIndex;
};

/// Blueprint for Radar.
struct RadarData : public ShipModuleData
{
	RadarData() :
		ShipModuleData()
	{
		dish.texName = "module/dish";
		dish.randomRot = true;
		dish.rate = 90;//deg/s
		dish.layer = GraphicsLayer::ShipAppendagesLower;
	}

	SpinnerData dish;

	///Create Radar object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Radar, RadarData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new RadarData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};



#endif // RADAR_HPP
