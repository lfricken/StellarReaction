#ifndef PLATING_HPP
#define PLATING_HPP

#include "ShipModule.hpp"

struct PlatingData;

class Plating : public ShipModule
{
public:
	Plating(const PlatingData& rData);
	virtual ~Plating();

protected:
private:
};


struct PlatingData : public ShipModuleData
{
	PlatingData() :
		ShipModuleData()
	{
		baseDecor.texName = "radar/radar_base.png";
		baseDecor.animSheetName = "radar/radar_base.acfg";
	}

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		PlatingData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Plating(copy);
	}
	virtual ModuleData* clone() const
	{
		return new PlatingData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, PlatingData);
};



#endif // PLATING_HPP