#ifndef Teleport_HPP
#define Teleport_HPP

#include "ShipModule.hpp"

struct TeleportData;

/// Allows the ship to teleport some distance.
class Teleport : public ShipModule
{
public:
	Teleport(const TeleportData& rData);
	virtual ~Teleport();
	///Actions to process on object after performing physics updates.
	virtual void postPhysUpdate();
	///Send a command to a target.
	void directive(const CommandInfo& commands);

protected:
private:
	float m_eConsump;
	float m_teleRange;
	float m_teleCooldown;
	Timer m_teleTimer;
};

/// Teleport Blueprint.
struct TeleportData : public ShipModuleData
{
	TeleportData() :
		ShipModuleData(),
		energyConsumption(5),
		teleRange(15),
		teleCooldown(20)
	{
	}

	float energyConsumption;
	float teleRange;
	float teleCooldown;
	///Create Teleport object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		TeleportData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Teleport(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new TeleportData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, TeleportData);
};



#endif // Teleport_HPP