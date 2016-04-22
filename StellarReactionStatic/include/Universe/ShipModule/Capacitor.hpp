#ifndef CAPACITOR_HPP
#define CAPACITOR_HPP

#include "ShipModule.hpp"

struct CapacitorData;

/*
* Capacitor Class:
* Extends ShipModule to implement capacitor game object.
* Capacitor game objects provide energy to the ship.
*/
class Capacitor : public ShipModule
{
public:
	Capacitor(const CapacitorData& rData);
	virtual ~Capacitor();
	///Actions to process on object when HealthState changes.
	virtual void setHealthStateHook(HealthState newState);

protected:
private:

	bool m_hasContributed;
	float m_storage;
};



struct CapacitorData : ShipModuleData
{
	CapacitorData() :
		ShipModuleData(),
		storage(100)
	{
		baseDecor.texName = "capacitor/capacitor_base.png";
		baseDecor.animSheetName = "capacitor/capacitor_base.acfg";
	}

	float storage;
	///Create Capacitor object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		CapacitorData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Capacitor(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new CapacitorData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, CapacitorData);
};


#endif // CAPACITOR_HPP
