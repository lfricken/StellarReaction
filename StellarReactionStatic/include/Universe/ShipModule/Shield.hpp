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
};

struct ShieldComponentData : public SensorData
{
	ShieldComponentData() :
		SensorData()
	{
		fixComp.isSensor = true;
		fixComp.shape = leon::Shape::Rectangle;
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

protected:
private:
	float m_eConsump;
	bool out_of_energy;
	const float consump_per_hit = 2.0;

	float radius = 10.0;

	ShieldComponent* shield;
};

struct ShieldData : public ShipModuleData
{
	ShieldData() : ShipModuleData(), energyConsumption(5)
	{
		baseDecor.texName = "shield/shield.png";
		baseDecor.animSheetName = "shield/shield.acfg";
	}

	float energyConsumption;

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