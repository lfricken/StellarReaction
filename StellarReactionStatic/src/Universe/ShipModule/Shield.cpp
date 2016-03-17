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

void ShieldComponentData::loadJson(const Json::Value& root)
{
	SensorData::loadJson(root);
}

/*
From this line and below pertains to the Shield module.
 */
Shield::Shield(const ShieldData& rData) : ShipModule(rData)
{
	m_eConsump = rData.energyConsumption;

	ShieldComponentData newShield;

	this->m_parentChunk->add(newShield);

	shield = dynamic_cast<ShieldComponent*>(this->m_parentChunk->getModuleList().back().get());
}

Shield::~Shield() {}

void Shield::directive(map<Directive, bool>& rIssues)
{
	if (rIssues[Directive::ShieldToggle])
	{
		cout << "Shield pressed." << endl;
	}
}

void ShieldData::loadJson(const Json::Value& root)
{
	if (!root["EnergyConsumption"].isNull())
		energyConsumption = root["EnergyConsumption"].asFloat();

	ShipModuleData::loadJson(root);
}