#include "Radar.hpp"

Radar::Radar(const RadarData& rData) : ShipModule(rData)
{
	m_dishIndex = m_decors.size();
	m_decors.push_back(sptr<GraphicsComponent>(new Spinner(rData.dish)));

	m_zoom = rData.zoomAddition;

	m_pZoomPool->changeValue(m_zoom);
	m_hasContributed = true;
}
Radar::~Radar()
{
	m_pZoomPool->changeValue(-m_zoom);
	m_hasContributed = false;
}
void Radar::setHealthStateHook(HealthState newState)
{
	if(!functioning())
		static_cast<Spinner*>(m_decors[m_dishIndex].get())->toggleOn(false);
	else
		static_cast<Spinner*>(m_decors[m_dishIndex].get())->toggleOn(true);

	if(m_hasContributed && !functioning())
	{
		m_pZoomPool->changeValue(-m_zoom);
		m_hasContributed = false;
	}
	else if(!m_hasContributed && functioning())
	{
		m_pZoomPool->changeValue(m_zoom);
		m_hasContributed = true;
	}
}

void Radar::stealthOn(bool toggle)
{
	ShipModule::stealthOn(toggle);
	m_decors[m_dishIndex]->setAlpha(50);
}