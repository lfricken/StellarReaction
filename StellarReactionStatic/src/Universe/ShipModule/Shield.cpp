#include "Shield.hpp"
#include "Controller.hpp"

using namespace std;

/*
From this line and to the second block comments pertains to the ShieldComponent SENSOR.
*/
ShieldComponent::ShieldComponent(const ShieldComponentData& rData) : Sensor(rData) {}
ShieldComponent::~ShieldComponent() {}

void ShieldComponent::entered(FixtureComponent* pOther)
{

}
void ShieldComponent::exited(FixtureComponent* pOther)
{

}
void ShieldComponent::input(std::string rCommand, sf::Packet rData)
{
	if(isEnabled())
	{
		if(rCommand == "damage")
		{
			int val;
			int cause;
			rData >> val >> cause;

			Message mes(cause, "hitShield", voidPacket, 0, false);
			game.getUniverse().getUniverseIO().recieve(mes);
		}
		else
			Sensor::input(rCommand, rData);
	}
}
void ShieldComponentData::loadJson(const Json::Value& root)
{
	SensorData::loadJson(root);
}

Shield::Shield(const ShieldData& rData) : ShipModule(rData)
{
	m_eConsump = rData.energyConsumption;

	ShieldComponentData newShield;
	newShield.fixComp.shape = leon::Shape::Circle;
	newShield.fixComp.size.x = rData.radius;
	newShield.fixComp.offset = rData.fixComp.offset;
	this->m_parentChunk->add(newShield);

	shield = dynamic_cast<ShieldComponent*>(this->m_parentChunk->getModuleList().back().get());
}
Shield::~Shield()
{

}
void Shield::prePhysUpdate()
{
	ShipModule::prePhysUpdate();

	Energy thisTickConsumption = m_eConsump*game.getUniverse().getTimeStep();//calculate energy consumption TODO
	if(!isFunctioning() || m_pEnergyPool->getValue() < thisTickConsumption)
	{
		shield->disable();
	}
	else
		shield->enable();

	if(shield->isEnabled())
	{
		m_pEnergyPool->changeValue(-thisTickConsumption);
		//consume this tick//calculate energy consumption TODO
	}
}
void Shield::directive(map<Directive, bool>& rIssues)
{
	if(rIssues[Directive::ShieldToggle])
	{
		cout << "Shield pressed." << endl;
	}
}
void ShieldData::loadJson(const Json::Value& root)
{
	if(!root["EnergyConsumption"].isNull())
		energyConsumption = root["EnergyConsumption"].asFloat();
	if(!root["ShieldRadius"].isNull())
		radius = root["ShieldRadius"].asFloat();

	ShipModuleData::loadJson(root);
}
