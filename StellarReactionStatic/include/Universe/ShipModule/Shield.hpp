#ifndef SHIELD_HPP
#define SHIELD_HPP

#include "ShipModule.hpp"
#include "Sensor.hpp"

class Shield;
struct ShieldData;
struct ShieldComponentData;
/*
* ShieldComponent Class:
* Extends Sensor to implement ShieldComponent game objects.
* ShieldComponent game objects block incoming projectiles.
*/
class ShieldComponent : public Sensor
{
public:
	ShieldComponent(const ShieldComponentData& rData);
	///Actions to process when fixture component enters our sensor. May be called multiple times in a single step.
	virtual void entered(FixtureComponent* pOther);
	///Actions to process when fixture component exits our sensor. May be called multiple times in a single step.
	virtual void exited(FixtureComponent* pOther);
protected:
	virtual void input(std::string rCommand, sf::Packet rData);
	Shield* m_pParentShieldModule;
};

struct ShieldComponentData : public SensorData
{
	ShieldComponentData() :
		SensorData()
	{
		title = "ShieldComponent";
		fixComp.isSensor = true;
		fixComp.shape = leon::Shape::Rectangle;
		fixComp.colCategory = Category::ShipForceField;
		fixComp.colMask = Mask::ShipForceField;
		fixComp.density = 0.f;

		startEnabled = false;
	}


	Shield* pParentShieldModule;

	///Create ShieldComponent object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		ShieldComponentData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new ShieldComponent(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ShieldComponentData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, ShieldComponentData);
};

/*
* Shield Class:
* Extends ShipModule to implement shield game objects.
* Shield game objects create shield components that block incoming projectiles.
*/

class Shield : public ShipModule
{
public:
	Shield(const ShieldData& rData);
	///Send command to a target.
	void directive(const CommandInfo& commands);
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();
	///After blocking a projectile, do we still have enough energy to maintain the shield?
	bool hitConsumption();

protected:

private:
	Timer m_toggleTimer;
	Timer m_consumptionTimer;
	
	float m_energyPerSecond;
	float m_energyPerHit;

	ShieldComponent* m_pShield;
};

struct ShieldData : public ShipModuleData
{
	ShieldData() : 
		ShipModuleData(),
		energyPerSecond(5),
		energyPerHit(1),
		radius(5),
		toggleFrequency(1)
	{
		baseDecor.texName = "shield/shield.png";
		baseDecor.animSheetName = "shield/shield.acfg";
	}

	///Energy consumed per second.
	float energyPerSecond;
	///Energy consumed per hit.
	float energyPerHit;
	///How big the shield is.
	float radius;
	///How frequently we can turn it on or off.
	float toggleFrequency;

	///Create Shield object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		ShieldData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Shield(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ShieldData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
	MyType(ModuleData, ShieldData);
};

#endif // SHIELD_HPP