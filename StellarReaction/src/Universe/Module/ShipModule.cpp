#include "ShipModule.hpp"

using namespace std;

ShipModule::ShipModule(const ShipModuleData& rData) : Module(rData), m_health(rData.health)
{
	m_decors.resize(1);
	m_decors[0] = sptr<GraphicsComponent>(new QuadComponent(rData.baseDecor));

	m_healthState = rData.initHealthState;
	m_functionsDamaged = rData.functionsDamaged;
	m_decors[0]->setPosition(m_fix.getCenter());
	m_decors[0]->setRotation(m_fix.getAngle());
}
ShipModule::~ShipModule()
{

}
void ShipModule::prePhysUpdate()
{

}
void ShipModule::postPhysUpdate()
{
	for(int i=0; i<(signed)m_decors.size(); ++i)
	{
		m_decors[i]->setPosition(m_fix.getCenter());
		m_decors[i]->setRotation(m_fix.getAngle());
	}
}

void ShipModule::pack(sf::Packet& rPacket)
{

}
void ShipModule::unpack(sf::Packet& rPacket)
{

}
void ShipModule::input(std::string rCommand, sf::Packet rData)
{
	const float damagedPercent = 0.25;

	if(rCommand == "damage")
	{
		int val;
		rData >> val;
		m_health.damage(val);
		m_io.event(EventType::Health, m_health.getHealth(), voidPacket);
		if(m_health.isDead())
			setHealthState(HealthState::Broken);
		else if(m_health.getHealthPercent() <= damagedPercent)//damaged percent
			setHealthState(HealthState::Damaged);
	}
	else if(rCommand == "heal")
	{
		int val;
		rData >> val;
		m_health.heal(val);
		m_io.event(EventType::Health, m_health.getHealth(), voidPacket);
		if(m_health.getHealthPercent() >= damagedPercent)
			setHealthState(HealthState::Nominal);
	}
	else
		Module::input(rCommand, rData);
}
bool ShipModule::functioning()//does this module still do its function
{
	if(m_healthState == HealthState::Nominal)
		return true;
	else if(m_healthState == HealthState::Broken)
		return false;
	else if(m_healthState == HealthState::Damaged)
	{
		if(m_functionsDamaged)
			return true;
		else
			return false;
	}
	cout << "\n" << FILELINE;
	return true;
}
void ShipModule::setHealthState(HealthState newState)
{
	///fsm needed?
	//nominal -> damaged
	//no effect

	//nominal -> broken
	//set colcat broken

	//damaged -> broken
	//set colcat broken

	//damaged -> nominal
	//set colcat normal

	//broken -> damaged
	//set colcat broken

	//broken -> nominal
	//set colcat normal

	//else no effect cause we didn't change state

	m_healthState = newState;

	if(newState == HealthState::Broken)
	{
		m_fix.setCategory(Category::ShipModuleBroke);
		m_decors[0]->setColor(sf::Color(255,0,0,255));///makes the sprite red when destroyed
		///WE SHOULD SET ALL SPRITES TO BE DAMAGED STATE
	}


	setHealthStateHook(newState);
}
void ShipModule::setHealthStateHook(HealthState newState)
{

}
