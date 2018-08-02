#include "Shield.hpp"
#include "Controller.hpp"
#include "Chunk.hpp"


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
	//	getGame()->getUniverse().getUniverseIO().recieve(mes);
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
			// we took the hit successfully
		}
		else // we ran out of energy
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
	assert(m_pShield != nullptr);

	auto shieldArt = rData.shieldArt;

	auto pixelRadius = Convert::universeToScreen(rData.radius);
	shieldArt.dimensions.x = pixelRadius;
	shieldArt.dimensions.y = pixelRadius;
	m_spShieldArt.reset(new QuadComponent(shieldArt));
}
void Shield::prePhysUpdate()
{
	ShipModule::prePhysUpdate();

	float thisTickConsumption = m_energyPerSecond*m_consumptionTimer.getTimeElapsed();
	float additionalCost = thisTickConsumption * (1 - functionalCapacity());
	
	if(m_pShield->isEnabled())
		(*ranges)[RangeList::Energy].changeValue(-thisTickConsumption);
}
bool Shield::hitConsumption()
{
	float additionalCost = m_energyPerHit * (1 - functionalCapacity());
	return (*ranges)[RangeList::Energy].changeValue(-(m_energyPerHit + additionalCost));
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





