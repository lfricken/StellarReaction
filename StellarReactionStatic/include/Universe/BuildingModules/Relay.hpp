#pragma once

#include "ShipModule.hpp"

struct RelayData;

/// <summary>
/// Module that creates energy.
/// </summary>
class Relay : public ShipModule
{
public:

	Relay(const RelayData& rData);
	virtual ~Relay();

protected:
	bool m_opposingTeams; // true if this change in resources is applied to the opposing teams
	Resources m_resourceChange; // how much income does this object add or subtract from its team?
private:

};

struct RelayData : ShipModuleData
{
	RelayData() :
		ShipModuleData(),
		opposingTeams(false)
	{
	}
	bool opposingTeams; // true if this change in resources is applied to the opposing teams
	Resources resourceChange; // how much income does this object add or subtract from its team just by existing?

	///Create Capacitor object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Relay, RelayData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new RelayData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};
