#pragma once

#include "Sensor.hpp"

struct GravityFieldData;

class GravityField : public Sensor
{
public:
	GravityField(const GravityFieldData& rData);
	~GravityField();

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

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		GravityFieldData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new GravityField(copy);
	}
	virtual ModuleData* clone() const
	{
		return new GravityFieldData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, GravityFieldData);
};

