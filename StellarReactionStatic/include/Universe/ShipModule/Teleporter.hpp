#ifndef Teleport_HPP
#define Teleport_HPP

#include "ShipModule.hpp"

struct TeleportData;

/// Allows the ship to teleport some distance.
class Teleporter
{
public:
	Teleporter(const TeleporterData& data);
	virtual ~Teleporter();

	/// <summary>
	/// Try and teleport.
	/// </summary>
	virtual void directive(const CommandInfo& commands);

protected:
private:
	ModuleParent* parent; // ship/chunk this is attached to

	float energyConsumptionPerDist; // how much energy is consumed when you teleport
	float cooldown; // time in seconds till you can teleport again

	Timer m_cooldownTimer; // lets us know when we can teleport again
};

/// Teleport Blueprint.
struct TeleporterData
{
	TeleporterData() :
		energyConsumptionPerDist(2),
		cooldown(10)
	{
	}

	ModuleParent* parent; // ship/chunk this is attached to

	float energyConsumptionPerDist; // how much energy is consumed when you teleport
	float cooldown; // time in seconds till you can teleport again

	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, TeleportData);
};



#endif // Teleport_HPP