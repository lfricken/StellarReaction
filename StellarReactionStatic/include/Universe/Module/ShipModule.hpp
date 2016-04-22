#ifndef SHIPMODULE_HPP
#define SHIPMODULE_HPP

#include "Module.hpp"
#include "QuadComponent.hpp"
#include "Health.hpp"

enum class HealthState
{
	Nominal,//the module is healthy and works
	Damaged,//the module is damaged and may not work
	Broken,//the module is severely damaged and does not work
};


struct ShipModuleData;

/*
* ShipModule Class:
* Extends Module to implement ship modules.
*/


class ShipModule : public Module ///maybe we should make this virtual to ensure this never gets created...
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

	const int alpha_stealth_on = 50;
	const int alpha_stealth_off = 255;
private:
};



struct ShipModuleData : public ModuleData
{
	ShipModuleData() :
		baseDecor(),
		health(),
		initHealthState(HealthState::Nominal),///NOT IN DATA
		functionsDamaged(false)///NOT IN DATA
	{
		baseDecor.layer = GraphicsLayer::ShipModules;
	}

	QuadComponentData baseDecor;
	HealthData health;
	HealthState initHealthState;
	bool functionsDamaged;//does this module still function when damaged?
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
};



#endif // SHIPMODULE_HPP
