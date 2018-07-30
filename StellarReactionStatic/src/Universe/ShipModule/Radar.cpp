#include "Radar.hpp"

void RadarData::loadJson(const Json::Value& root)
{
	ShipModuleData::loadJson(root);
}

Radar::Radar(const RadarData& rData) : ShipModule(rData)
{
	m_dishIndex = m_decors.size();
	m_decors.push_back(sptr<GraphicsComponent>(new Spinner(rData.dish)));
}
Radar::~Radar()
{

}
void Radar::setHealthStateHook(HealthState newState)
{
	if(getHealthState() == HealthState::CriticallyDamaged)
	{
		static_cast<Spinner*>(m_decors[m_dishIndex].get())->toggleOn(false);
	}
	else
	{
		static_cast<Spinner*>(m_decors[m_dishIndex].get())->toggleOn(true);
	}
}

void Radar::toggleStealth(bool toggle)
{
	ShipModule::toggleStealth(toggle);
	if (toggle)
		m_decors[m_dishIndex]->setAlpha(alpha_stealth_on);
	else
		m_decors[m_dishIndex]->setAlpha(alpha_stealth_off);
}

