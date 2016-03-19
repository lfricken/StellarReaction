#ifndef SHIELD_HPP
#define SHIELD_HPP

#include "ShipModule.hpp"
#include "Sensor.hpp"

struct ShieldData;
struct ShieldComponentData;

class ShieldComponent : public Sensor
{
public:
	ShieldComponent(const ShieldComponentData& rData);
	virtual ~ShieldComponent();

	virtual void entered(FixtureComponent* pOther);
	virtual void exited(FixtureComponent* pOther);
protected:
	virtual void input(std::string rCommand, sf::Packet rData);
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
	}

	bool startEnabled;

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
	virtual ~Shield();

	void directive(std::map<Directive, bool>& rIssues);

	virtual void prePhysUpdate();

protected:

private:

	
	float m_eConsump;
	bool out_of_energy;
	const float consump_per_hit = 2.0;

	ShieldComponent* shield;
};

struct ShieldData : public ShipModuleData
{
	ShieldData() : 
		ShipModuleData(),
		energyConsumption(1),
		radius(10)
	{
		baseDecor.texName = "shield/shield.png";
		baseDecor.animSheetName = "shield/shield.acfg";
	}

	float energyConsumption;//energy consumed per second
	float radius;//how big the shield is

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