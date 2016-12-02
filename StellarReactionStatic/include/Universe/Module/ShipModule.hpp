#ifndef SHIPMODULE_HPP
#define SHIPMODULE_HPP

#include "Module.hpp"
#include "QuadComponent.hpp"
#include "Health.hpp"
#include "Sound.hpp"

enum class HealthState
{
	Nominal,//the module is healthy and works
	Damaged,//the module is damaged and may not work
	Broken,//the module is severely damaged and does not work
};

struct ShipModuleData;

/// Represent functional components of a ship, which go on a Chunk.
class ShipModule : public Module
{
public:
	ShipModule(const ShipModuleData& rData);
	virtual ~ShipModule() = 0;
	//Return health state.
	const Health getHealth();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();
	///Actions to process on object after performing physics updates.
	virtual void postPhysUpdate();
	///Should this module still work?
	bool isFunctioning();
	///Set the health state of this module.
	void setHealthState(HealthState newState);
	///Actions to process on object when HealthState changes.
	virtual void setHealthStateHook(HealthState newState);
	///Toggle stealth state.
	virtual void toggleStealth(bool toggle);
	///Heal to full health.
	virtual void healToMax();

protected:
	virtual void input(std::string rCommand, sf::Packet rData);
	virtual void pack(sf::Packet& rPacket);
	virtual void unpack(sf::Packet& rPacket);
	virtual void f_died();

	Health m_health;
	HealthState m_healthState;
	bool m_functionsDamaged;

	std::vector<sptr<GraphicsComponent> > m_decors;
	///index in m_decors where the module sprite is.
	int m_baseDecor;
	///index in m_decors where the hit sprite is.
	int m_hitDecorIndex;
	///index in m_decors where the exposion sprite is.
	int m_explosionIndex;
	/// Death Sound.
	leon::Sound m_deathSound;//when last shot is taken

	const int alpha_stealth_on = 50;
	const int alpha_stealth_off = 255;
private:
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
		baseDecor.texName = "default.png";
		baseDecor.animSheetName = "default.acfg";
		baseDecor.layer = GraphicsLayer::ShipModules;
		deathSound.name = "ExplodeSmall.wav";
	}

	QuadComponentData baseDecor;
	HealthData health;
	HealthState initHealthState;
	bool functionsDamaged;//does this module still function when damaged?
	leon::Sound deathSound;

	///Returns NULL and prints the file and line that attempted to instantiate this virtual class.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		std::cout << FILELINE;
		return NULL;
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ShipModuleData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, ShipModuleData);

protected:
	virtual void inherit(const ShipModuleData& parent)
	{
		*this = parent;
	}
};



#endif // SHIPMODULE_HPP
