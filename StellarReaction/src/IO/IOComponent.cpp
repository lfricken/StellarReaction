#include "IOComponent.hpp"

using namespace std;

IOComponent::~IOComponent()
{
	m_rManager.free(m_ioManPosition);
}
void IOComponent::event(EventType type, int value, const sf::Packet& rData)
{
	m_eventer.event(type, value, rData);
}
void IOComponent::recieve(const std::string& rCommand, const sf::Packet& rData)
{
	m_cbFunction(rCommand, rData);
}
const std::string& IOComponent::getName() const
{
	return m_name;
}
int IOComponent::getPosition() const
{
	return m_ioManPosition;
}
