#include "Sensor.hpp"


void SensorData::loadJson(const Json::Value& root)
{
	ModuleData::loadJson(root);

	GETJSON(startEnabled);
	GETJSON(disabledColCategory);
}
Sensor::Sensor(const SensorData& rData) : Module(rData)
{
	m_enabledCollision = rData.fixComp.colCategory;
	m_disabledCollision = rData.disabledColCategory;

	toggleEnabled(rData.startEnabled);
}
Sensor::~Sensor()
{

}
void Sensor::prePhysUpdate()
{
	if(isEnabled())
		m_fix.setCategory(m_enabledCollision);
	else
		m_fix.setCategory(m_disabledCollision);
}
void Sensor::startContactCB(FixtureComponent* pOther)
{
	//add to our list
	m_guests.push_back(pOther);

	if(isEnabled())
	{
		sf::Packet enter;
		enter << pOther->getIOPos();
		m_io.event(EventType::OnEnter, m_guests.size(), enter);

		sf::Packet count;
		count << m_guests.size();
		m_io.event(EventType::OnCount, m_guests.size(), count);

		entered(pOther);
	}
}
void Sensor::endContactCB(FixtureComponent* pOther)
{
	//take away from our list
	for(auto it=m_guests.begin(); it!=m_guests.end(); ++it)
		if((*it) == pOther)
		{
			m_guests.erase(it);
			break;
		}

	if(isEnabled())
	{
		sf::Packet exit;
		exit << pOther->getIOPos();
		m_io.event(EventType::OnExit, m_guests.size(), exit);

		sf::Packet count;
		count << m_guests.size();
		m_io.event(EventType::OnCount, m_guests.size(), count);

		exited(pOther);
	}
}
void Sensor::entered(FixtureComponent* pOther)
{

}
void Sensor::exited(FixtureComponent* pOther)
{

}
void Sensor::toggleEnabled(bool enabled)
{
	m_enabled = enabled;
}
bool Sensor::isEnabled() const
{
	return m_enabled;
}







