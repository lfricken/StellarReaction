#pragma once

#include "Sensor.hpp"

struct TriggerSensorData;

class TriggerSensor : public Sensor
{
public:
	TriggerSensor(const TriggerSensorData& rData);
	~TriggerSensor();

	virtual void prePhysUpdate();

protected:
	float m_damage;
	float m_period;
	Timer m_damageTimer;
private:
};

struct TriggerSensorData : public SensorData
{
	TriggerSensorData() :
		dmg(50),
		period(3)
	{

	}

	float dmg;
	float period;

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		TriggerSensorData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new TriggerSensor(copy);
	}
	virtual ModuleData* clone() const
	{
		return new TriggerSensorData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, TriggerSensorData);
};

