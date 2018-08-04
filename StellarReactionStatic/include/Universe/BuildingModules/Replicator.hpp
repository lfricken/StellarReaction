#pragma once

#include "ShipModule.hpp"

struct ReplicatorData;

/// <summary>
/// Module that creates ships.
/// </summary>
class Replicator : public ShipModule
{
public:

	Replicator(const ReplicatorData& rData);
	virtual ~Replicator();

protected:
private:

};

struct ReplicatorData : ShipModuleData
{
	ReplicatorData() :
		ShipModuleData()

	{
	}

	///Create Capacitor object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Replicator, ReplicatorData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ReplicatorData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};
