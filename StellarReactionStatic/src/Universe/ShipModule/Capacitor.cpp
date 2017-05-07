#include "Capacitor.hpp"


void CapacitorData::loadJson(const Json::Value& root)
{
	GETJSON(storage);

	ShipModuleData::loadJson(root);
}
Capacitor::Capacitor(const CapacitorData& rData) : ShipModule(rData)
{
	m_storage = rData.storage;

	m_poolChanger.tryContributeMax(m_pEnergyPool, m_storage);
}
Capacitor::~Capacitor()
{
	m_poolChanger.tryRemoveMax(m_pEnergyPool, m_storage);
}
void Capacitor::setHealthStateHook(HealthState newState)
{
	if(this->isFunctioning())
		m_poolChanger.tryContributeMax(m_pEnergyPool, m_storage);
	else
		m_poolChanger.tryRemoveMax(m_pEnergyPool, m_storage);
}





