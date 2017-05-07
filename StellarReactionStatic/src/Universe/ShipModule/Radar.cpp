#include "Radar.hpp"

void RadarData::loadJson(const Json::Value& root)
{
	GETJSON(zoomAddition);

	ShipModuleData::loadJson(root);
}

Radar::Radar(const RadarData& rData) : ShipModule(rData)
{
	m_dishIndex = m_decors.size();
	m_decors.push_back(sptr<GraphicsComponent>(new Spinner(rData.dish)));
	m_zoom = rData.zoomAddition;

	m_poolChanger.tryContributeValue(m_pZoomPool, m_zoom);
}
Radar::~Radar()
{
	m_poolChanger.tryRemoveValue(m_pZoomPool, m_zoom);
}
void Radar::setHealthStateHook(HealthState newState)
{
	if(isFunctioning())
	{
		static_cast<Spinner*>(m_decors[m_dishIndex].get())->toggleOn(true);
		m_poolChanger.tryContributeValue(m_pZoomPool, m_zoom);
	}
	else
	{
		static_cast<Spinner*>(m_decors[m_dishIndex].get())->toggleOn(false);
		m_poolChanger.tryRemoveValue(m_pZoomPool, m_zoom);
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

