#pragma once

#include "Module.hpp"

struct SensorData;

/*
* Sensor Class:
* Extends Module to implement sensor game object.
* Sensor game objects detect if other objects enter their sensing radius.
*/
class Sensor : public Module
{
public:
	Sensor(const SensorData& rData);
	virtual ~Sensor();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();
	///Actions to process when fixture component enters our sensor. May be called multiple times in a single step.
	virtual void entered(FixtureComponent* pOther);
	///Actions to process when fixture component exits our sensor. May be called multiple times in a single step.
	virtual void exited(FixtureComponent* pOther);
	///Toggle the state (enabled/disabled).
	void toggleEnabled(bool enabled);
	///Return the state.
	bool isEnabled() const;

protected:
	void startContactCB(FixtureComponent* pOther) final;
	void endContactCB(FixtureComponent* pOther) final;

	std::vector<FixtureComponent*> m_guests;
private:
	bool m_enabled;
};



struct SensorData : public ModuleData
{
	SensorData() :
		startEnabled(true)
	{
		fixComp.isSensor = true;
		fixComp.density = 0.f;
	}

	bool startEnabled;
	///Create Sensor object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		SensorData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Sensor(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new SensorData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, SensorData);
};
