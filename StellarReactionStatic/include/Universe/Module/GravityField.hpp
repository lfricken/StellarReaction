#pragma once

#include "Sensor.hpp"

struct GravityFieldData;


///GravityFields pull other objects into them by applying forces within their Sensor.
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

/// Initialize GravityField.
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
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<GravityField, GravityFieldData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new GravityFieldData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};

