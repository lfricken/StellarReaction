#pragma once

#include "Module.hpp"

struct SensorData;


/// <summary>
/// Detects other objects that overlap their region (usualy circular). Do not cause rebounding collisions.
/// </summary>
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

	List<FixtureComponent*> m_guests;
private:
	bool m_enabled;
	Category m_enabledCollision;
	Category m_disabledCollision;
};


/// Initialize Sensor.
struct SensorData : public ModuleData
{
	SensorData() :
		startEnabled(true),
		disabledColCategory(Category::None)
	{
		fixComp.isSensor = true;
		fixComp.mass = 0.f;
		fixComp.colCategory = Category::Trigger;
		fixComp.colMask = Mask::Trigger;
	}

	bool startEnabled;
	Category disabledColCategory;
	///Create Sensor object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Sensor, SensorData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new SensorData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};
