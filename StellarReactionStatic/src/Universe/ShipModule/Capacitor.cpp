#include "Capacitor.hpp"


void CapacitorData::loadJson(const Json::Value& root)
{
	GETJSON(storage);

	ShipModuleData::loadJson(root);
}
Capacitor::Capacitor(const CapacitorData& rData) : ShipModule(rData)
{
	m_storage = rData.storage;

	m_pEnergyPool->changeMax(m_storage);
	m_hasContributed = true;
}
Capacitor::~Capacitor()
{
	m_pEnergyPool->changeMax(-m_storage);
	m_hasContributed = false;
}
void Capacitor::setHealthStateHook(HealthState newState)
{
	if(m_hasContributed && !isFunctioning())
	{
		m_pEnergyPool->changeMax(-m_storage);
		m_hasContributed = false;
	}
	else if(!m_hasContributed && isFunctioning())
	{
		m_pEnergyPool->changeMax(m_storage);
		m_hasContributed = true;
	}
}
