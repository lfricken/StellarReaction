#ifndef SHIELD_HPP
#define SHIELD_HPP

#include "ShipModule.hpp"
#include "Sensor.hpp"

class Shield;
struct ShieldData;
struct ShieldComponentData;

class ShieldComponent : public Sensor
{
public:
	ShieldComponent(const ShieldComponentData& rData);

	virtual void entered(FixtureComponent* pOther);
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


	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		ShieldComponentData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new ShieldComponent(copy);
	}
	virtual ModuleData* clone() const
	{
		return new ShieldComponentData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, ShieldComponentData);
};

class Shield : public ShipModule
{
public:
	Shield(const ShieldData& rData);

	void directive(const CommandInfo& commands);

	virtual void prePhysUpdate();
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


	float energyPerSecond;//energy consumed per second
	float energyPerHit;//energy consumed per hit
	float radius;//how big the shield is
	float toggleFrequency;//how frequently we can turn it on or off


	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		ShieldData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Shield(copy);
	}
	virtual ModuleData* clone() const
	{
		return new ShieldData(*this);
	}
	virtual void loadJson(const Json::Value& root);
	MyType(ModuleData, ShieldData);
};

#endif // SHIELD_HPP