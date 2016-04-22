#pragma once

#include "Sensor.hpp"

struct GravityFieldData;

/*
* GravityField Class:
* Extends Sensor to implement gravity field game object.
*/

class GravityField : public Sensor
{
public:
	GravityField(const GravityFieldData& rData);
	~GravityField();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();

protected:
	float m_radius;
	float m_maxForce;
	float m_minForce;
private:
};

struct GravityFieldData : public SensorData
{
	GravityFieldData() :
		maxForce(50),
		minForce(5)
	{

	}

	float maxForce;
	float minForce;
	///Create GravityField object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		GravityFieldData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new GravityField(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new GravityFieldData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, GravityFieldData);
};

