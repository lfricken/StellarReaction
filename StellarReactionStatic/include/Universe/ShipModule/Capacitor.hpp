#ifndef CAPACITOR_HPP
#define CAPACITOR_HPP

#include "ShipModule.hpp"

struct CapacitorData;

/// Stores energy for a Ship.
class Capacitor : public ShipModule
{
public:
	Capacitor(const CapacitorData& rData);
	virtual ~Capacitor();
	///Actions to process on object when HealthState changes.
	virtual void setHealthStateHook(HealthState newState);

protected:
private:

	float m_storage;
};


/// Blueprint for Capacitor.
struct CapacitorData : ShipModuleData
{
	CapacitorData() :
		ShipModuleData(),
		storage(100)
	{
		baseDecor.texName = "capacitor/capacitor_base";
	}

	float storage;
	///Create Capacitor object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Capacitor, CapacitorData>(params, this);
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
