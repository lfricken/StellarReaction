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

class ShipModule : public Module ///maybe we should make this virtual to ensure this never gets created...
{
public:
	ShipModule(const ShipModuleData& rData);
	virtual ~ShipModule() = 0;

	const Health getHealth();

	virtual void prePhysUpdate();
	virtual void postPhysUpdate();

	bool isFunctioning();//should this module still work?
	void setHealthState(HealthState newState);
	virtual void setHealthStateHook(HealthState newState);
	virtual void toggleStealth(bool toggle);
	int getControlGroup() const;
	void setControlGroup(int control_group);
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
	int m_baseDecor;//index in m_decors where the module sprite is
	int m_hitDecorIndex;//index in m_decors where the hit sprite is
	int m_explosionIndex;//index in m_decors where the exposion sprite is

	int m_controlGroup;
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

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		std::cout << FILELINE;
		return NULL;
	}
	virtual ModuleData* clone() const
	{
		return new ShipModuleData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, ShipModuleData);
};



#endif // SHIPMODULE_HPP
