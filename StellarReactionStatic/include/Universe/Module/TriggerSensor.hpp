#pragma once

#include "Sensor.hpp"

struct TriggerSensorData;

/*
* TriggerSensor Class:
* Extends Sensor to implement TriggerSensor game objects.
* TriggerSensor game objects damage objects that enter their sensing radius.
*/
class TriggerSensor : public Sensor
{
public:
	TriggerSensor(const TriggerSensorData& rData);
	~TriggerSensor();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();

protected:
	int m_minDamage;
	int m_maxDamage;
	float m_period;

	float m_radius;
	Timer m_damageTimer;
private:
};

struct TriggerSensorData : public SensorData
{
	TriggerSensorData() :
		minDamage(10),
		maxDamage(100),
		period(3)
	{

	}

	int maxDamage;
	int minDamage;
	float period;
	///Create CaptureGraphics object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		TriggerSensorData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new TriggerSensor(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new TriggerSensorData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, TriggerSensorData);
};

