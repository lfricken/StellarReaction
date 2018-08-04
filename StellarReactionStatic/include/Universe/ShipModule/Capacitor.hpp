#pragma once

#include "ShipModule.hpp"

struct CapacitorData;

/// Stores energy for a Ship.
class Capacitor : public ShipModule
{
public:
	Capacitor(const CapacitorData& rData);
	virtual ~Capacitor();

protected:
private:

};


/// Blueprint for Capacitor.
struct CapacitorData : ShipModuleData
{
	CapacitorData() :
		ShipModuleData()
	{
		baseDecor.texName = "capacitor/capacitor_base";
	}

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
};

