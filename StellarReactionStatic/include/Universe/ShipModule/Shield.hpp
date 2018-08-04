#ifndef SHIELD_HPP
#define SHIELD_HPP

#include "ShipModule.hpp"
#include "Sensor.hpp"
#include "Convert.hpp"

class Shield;
struct ShieldData;
struct ShieldComponentData;



/// Designed to block incoming projectiles. The invisible part.
class ShieldComponent : public Sensor
{
public:
	ShieldComponent(const ShieldComponentData& rData);
	///Actions to process when fixture component enters our sensor. May be called multiple times in a single step.
	virtual void entered(FixtureComponent* pOther);
	///Actions to process when fixture component exits our sensor. May be called multiple times in a single step.
	virtual void exited(FixtureComponent* pOther);
protected:
	virtual void input(String rCommand, sf::Packet rData);
	Shield* m_pParentShieldModule;
};

/// Initialize Shield.
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
		fixComp.mass = 0.f;

		startEnabled = false;
	}


	Shield* pParentShieldModule;

	///Create ShieldComponent object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<ShieldComponent, ShieldComponentData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ShieldComponentData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};

/// ShipModule that controls a ShieldComponent.
/// Works kind of wierd, it has to add a new component onto the ship called a Shield Component
class Shield : public ShipModule
{
public:
	Shield(const ShieldData& rData);
	///Send command to a target.
	void directive(const CommandInfo& commands);
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();

	void enableShield();
	void disableShield();


	virtual void postPhysUpdate();

protected:
	/// <summary>
	/// Causes all shields on the ship to disable.
	/// </summary>
	void triggerGroupDisable();
	/// <summary>
	/// After blocking a projectile, do we still have enough energy to maintain the shield?
	/// </summary>
	bool hitConsumption();
private:
	Timer m_toggleTimer;
	Timer m_consumptionTimer;

	float m_energyPerSecond;
	float m_energyPerHit;

	ShieldComponent* m_pShield;
	sptr<QuadComponent> m_spShieldArt;

	friend class ShieldComponent;
};

/// Blueprint for a Shield.
struct ShieldData : public ShipModuleData
{
	ShieldData() :
		ShipModuleData(),
		energyPerSecond(5),
		energyPerHit(1),
		radius(5),
		toggleFrequency(1)
	{
		baseDecor.texName = "module/shield";
		shieldArt.dimensions = (sf::Vector2f)Convert::universeToScreen(Vec2(5.f, 5.f));
		shieldArt.texName = "module/barrier";
	}

	///Energy consumed per second.
	float energyPerSecond;
	///Energy consumed per hit.
	float energyPerHit;
	
	float radius; // How big the shield is.
	///How frequently we can turn it on or off.
	float toggleFrequency;

	QuadComponentData shieldArt;

	///Create Shield object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Shield, ShieldData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ShieldData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};

#endif // SHIELD_HPP