#include "Shield.hpp"
#include "Controller.hpp"

using namespace std;

void ShieldComponentData::loadJson(const Json::Value& root)
{
	SensorData::loadJson(root);
}
ShieldComponent::ShieldComponent(const ShieldComponentData& rData) : Sensor(rData)
{
	m_pParentShieldModule = rData.pParentShieldModule;
}
void ShieldComponent::entered(FixtureComponent* pOther)
{
	if(m_pParentShieldModule->hitConsumption())
	{
		sf::Packet cause;
		int32_t ioPos = m_io.getPosition();
		cause << ioPos;
		Message mes(pOther->getIOPos(), "hitShield", cause, 0, false);
		game.getUniverse().getUniverseIO().recieve(mes);
	}
	else
		this->toggleEnabled(false);
}
void ShieldComponent::exited(FixtureComponent* pOther)
{

}
void ShieldComponent::input(std::string rCommand, sf::Packet rData)
{

}




void ShieldData::loadJson(const Json::Value& root)
{
	GETJSON(energyPerSecond);
	GETJSON(energyPerHit);
	GETJSON(radius);
	GETJSON(toggleFrequency);

	ShipModuleData::loadJson(root);
}
Shield::Shield(const ShieldData& rData) : ShipModule(rData)
{
	m_energyPerSecond = rData.energyPerSecond;
	m_energyPerHit = rData.energyPerHit;
	m_toggleTimer.setCountDown(rData.toggleFrequency);
	m_toggleTimer.restartCountDown();

	ShieldComponentData newShield;
	newShield.pParentShieldModule = this;
	newShield.startEnabled = false;
	newShield.fixComp.shape = leon::Shape::Circle;
	newShield.fixComp.size.x = rData.radius;
	newShield.fixComp.offset = rData.fixComp.offset;
	this->m_parentChunk->add(newShield);

	m_pShield = dynamic_cast<ShieldComponent*>(this->m_parentChunk->getModuleList().back().get());
}
void Shield::prePhysUpdate()
{
	ShipModule::prePhysUpdate();

	Energy thisTickConsumption = m_energyPerSecond*m_consumptionTimer.getTimeElapsed();
	if(!isFunctioning() || m_pEnergyPool->getValue() < thisTickConsumption)
		m_pShield->toggleEnabled(false);

	if(m_pShield->isEnabled())
		m_pEnergyPool->changeValue(-thisTickConsumption);
}
bool Shield::hitConsumption()
{
	m_pEnergyPool->changeValue(-m_energyPerHit);
	return (m_pEnergyPool->getValue() > m_energyPerHit);
}
void Shield::directive(map<Directive, bool>& rIssues)
{
	if(rIssues[Directive::Shield])
	{
		if(m_toggleTimer.isTimeUp())
		{
			m_toggleTimer.restartCountDown();
			m_pShield->toggleEnabled(!m_pShield->isEnabled());
		}
	}
}
