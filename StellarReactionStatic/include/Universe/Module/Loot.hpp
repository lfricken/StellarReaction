#pragma once

#include "Sensor.hpp"
#include "QuadComponent.hpp"

struct LootData;
enum class Team;


/// Detects when colliding with something a potentially dealing damage.
class Loot : public Sensor
{
public:
	Loot(const LootData& rData);
	virtual ~Loot();
	///Actions to process on object after performing physics updates.
	void postPhysUpdate();
	///Actions to process when fixture component enters our sensor. May be called multiple times in a single step.
	virtual void entered(FixtureComponent* pOther);
	///Actions to process when fixture component exits our sensor. May be called multiple times in a single step.
	virtual void exited(FixtureComponent* pOther);
	///Input commands to this object.
	virtual void input(String rCommand, sf::Packet rData);

protected:
	Resources m_resources;
	/// <summary>
	/// True if this loot has already been picked up by someone.
	/// </summary>
	bool m_used;

private:

};

/// Initialize Loot
struct LootData : public SensorData
{
	LootData() :
		SensorData()
	{
		for(auto it = resources.m_resourceValues.begin(); it != resources.m_resourceValues.end(); ++it)
		{
			it->second = 1;
		}

		fixComp.isSensor = true;
		fixComp.shape = leon::Shape::Rectangle;
		fixComp.colCategory = Category::Projectile;
		fixComp.colMask = Mask::Projectile;
	}

	Resources resources;

	///Create Loot object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Loot, LootData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new LootData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};

