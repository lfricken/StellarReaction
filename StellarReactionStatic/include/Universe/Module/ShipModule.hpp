#pragma once

#include "Module.hpp"
#include "QuadComponent.hpp"
#include "Health.hpp"
#include "Sound.hpp"
#include "Debugging.hpp"

enum class UpgradeType;
struct ShipModuleData;

/// <summary>
/// Represent functional components of a ship, which go on a Chunk.
/// </summary>
class ShipModule : public Module
{
public:
	ShipModule(const ShipModuleData& rData);
	virtual ~ShipModule() = 0; // prevent anyone from instantiating this object
	//Return health state.
	const Health getHealth();
	/// <summary>
	/// Is this module broken or damaged?
	/// </summary>
	HealthState getHealthState() const;
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();
	///Actions to process on object after performing physics updates.
	virtual void postPhysUpdate();
	/// <summary>
	/// Returns as a percentage how functional this module is.
	/// </summary>
	virtual float functionalCapacity();
	///Set the health state of this module.
	void setHealthState(HealthState newState);
	///Actions to process on object when HealthState changes.
	virtual void setHealthStateHook(HealthState newState);
	///Toggle stealth state.
	virtual void toggleStealth(bool toggle);
	///Heal to full health.
	virtual void healToMax();

	Health m_health;
	Vec2 getNearestNeighbor(List<Vec2>& positions);

protected:
	virtual void input(String rCommand, sf::Packet rData);
	virtual void pack(sf::Packet& rPacket);
	virtual void unpack(sf::Packet& rPacket);
	virtual void f_died();

	HealthState m_healthState;
	bool m_functionsDamaged;

	List<sptr<GraphicsComponent> > m_decors;
	///index in m_decors where the hit sprite is.
	int m_hitDecorIndex;
	///index in m_decors where the exposion sprite is.
	int m_explosionIndex;
	/// Death Sound.
	leon::Sound m_deathSound;//when last shot is taken

	const int alpha_stealth_on = 50;
	const int alpha_stealth_off = 255;
private:

	void changeHealthState(int ioPosOfDealer);
	bool isValidDamageSource(int damage, Team damagingTeam);
	void moduleDamageGraphics();
};

/// Initialize ShipModule.
struct ShipModuleData : public ModuleData
{
	ShipModuleData() :
		baseDecor(),
		health(),
		initHealthState(HealthState::Nominal),///NOT IN DATA
		functionsDamaged(false)///NOT IN DATA
	{
		baseDecor.texName = "default";
		baseDecor.layer = GraphicsLayer::ShipModules;
		deathSound.name = "ExplodeSmall.wav";
	}

	QuadComponentData baseDecor;
	HealthData health;
	HealthData startHealth;
	HealthState initHealthState;
	bool functionsDamaged;//does this module still function when damaged?
	leon::Sound deathSound;

	///Returns nullptr and prints the file and line that attempted to instantiate this virtual class.
	virtual Module* generate(GenerateParams params) const
	{
		WARNING;
		return nullptr;
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ShipModuleData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	virtual void applyUpgrade(UpgradeType type);

protected:

	virtual void inherit(const ShipModuleData& parent)
	{
		*this = parent;
	}
};

