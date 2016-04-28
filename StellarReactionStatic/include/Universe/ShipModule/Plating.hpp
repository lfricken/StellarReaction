#ifndef PLATING_HPP
#define PLATING_HPP

#include "ShipModule.hpp"

struct PlatingData;

/// Designed to take damage and have high Armor.
class Plating : public ShipModule
{
public:
	Plating(const PlatingData& rData);
	virtual ~Plating();

protected:
private:
};

/// Blueprint for Plating.
struct PlatingData : public ShipModuleData
{
	PlatingData() :
		ShipModuleData()
	{
		baseDecor.texName = "radar/radar_base.png";
		baseDecor.animSheetName = "radar/radar_base.acfg";
	}
	///Create CaptureGraphics object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		PlatingData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Plating(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new PlatingData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, PlatingData);
};



#endif // PLATING_HPP