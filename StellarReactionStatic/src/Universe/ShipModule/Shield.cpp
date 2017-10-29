#include "Shield.hpp"
#include "Controller.hpp"



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
	//if(m_pParentShieldModule->hitConsumption())
	//{
	//	sf::Packet cause;
	//	int32_t ioPos = m_io.getPosition();
	//	cause << ioPos;
	//	Message mes(pOther->getIOPos(), "hitShield", cause, 0, false);
	//	game.getUniverse().getUniverseIO().recieve(mes);
	//}
	//else
	//	this->toggleEnabled(false);
}
void ShieldComponent::exited(FixtureComponent* pOther)
{

}
void ShieldComponent::input(String rCommand, sf::Packet rData)
{
	if(rCommand == "damage")
	{
		if(m_pParentShieldModule->hitConsumption())
		{

		}
		else
			m_pParentShieldModule->triggerGroupDisable();
	}
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
	this->m_parent->add(newShield);

	m_pShield = dynamic_cast<ShieldComponent*>(this->m_parent->getModuleList().back().get());
	assert(m_pShield != NULL);

	auto shieldArt = rData.shieldArt;
	shieldArt.dimensions.x = leon::toScreen(rData.radius);
	shieldArt.dimensions.y = leon::toScreen(rData.radius);
	m_spShieldArt.reset(new QuadComponent(shieldArt));
}
void Shield::prePhysUpdate()
{
	ShipModule::prePhysUpdate();

	float thisTickConsumption = m_energyPerSecond*m_consumptionTimer.getTimeElapsed();
	if(!isFunctioning())
		disableShield();

	if(m_pShield->isEnabled())
		(*ranges)[RangeList::Energy].changeValue(-thisTickConsumption);
}
bool Shield::hitConsumption()
{
	return (*ranges)[RangeList::Energy].tryChange(-m_energyPerHit);
}
void Shield::directive(const CommandInfo& commands)
{

}
void Shield::enableShield()
{
	m_pShield->toggleEnabled(true);
}
void Shield::disableShield()
{
	m_pShield->toggleEnabled(false);
}
void Shield::triggerGroupDisable()
{
	int target = m_parent->thisAsChunk()->m_io.getPosition();
	Message off(target, "disableShields", voidPacket, 0, false);
	Message::SendUniverse(off);
}
void Shield::postPhysUpdate()
{
	ShipModule::postPhysUpdate();

	m_spShieldArt->setPosition(m_fix.getCenter());

	if(m_pShield->isEnabled())
	{
		m_spShieldArt->setAlpha(255);
	}
	else
		m_spShieldArt->setAlpha(0);
}