#include "Sensor.hpp"

using namespace std;

Sensor::Sensor(const SensorData& rData) : Module(rData)
{
	m_enabled = rData.startEnabled;
}
Sensor::~Sensor()
{

}
void Sensor::prePhysUpdate()
{

}
void Sensor::startContactCB(FixtureComponent* pOther)
{
	//add to our list
	m_guests.push_back(pOther);

	if(m_enabled)
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

	if(m_enabled)
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
/// <summary>
/// Called for each fixture that enters us this step
/// </summary>
void Sensor::entered(FixtureComponent* pOther)
{

}
/// <summary>
/// Called for each fixture that exits us this step
/// </summary>
void Sensor::exited(FixtureComponent* pOther)
{

}
