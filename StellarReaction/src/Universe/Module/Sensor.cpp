#include "Sensor.hpp"

using namespace std;

Sensor::Sensor(const SensorData& rData) : Module(rData)
{

}
Sensor::~Sensor()
{

}
void Sensor::prePhysUpdate()
{

}
void Sensor::startContactCB(FixtureComponent* pOther)
{
	m_guests.push_back(pOther);
	sf::Packet enter;
	enter << pOther;//TODO THIS IS BAD, YOU CANT DO A POINTER, it gets casted
	m_io.event(EventType::OnEnter, m_guests.size(), enter);

	sf::Packet count;
	count << m_guests.size();
	m_io.event(EventType::OnCount, m_guests.size(), count);

	entered(pOther);
}
void Sensor::endContactCB(FixtureComponent* pOther)
{
	for(auto it=m_guests.begin(); it!=m_guests.end(); ++it)
		if((*it) == pOther)
		{
			m_guests.erase(it);
			break;
		}

	sf::Packet exit;
	exit << pOther;//TODO THIS IS BAD, YOU CANT DO A POINTER, it gets casted
	m_io.event(EventType::OnExit, m_guests.size(), exit);

	sf::Packet count;
	count << m_guests.size();
	m_io.event(EventType::OnCount, m_guests.size(), count);

	exited(pOther);
}
void Sensor::entered(FixtureComponent* pOther)
{

}
void Sensor::exited(FixtureComponent* pOther)
{

}
