#include "Resources.hpp"
#include "Team.hpp"
#include "Message.hpp"

Resources::Resources()
{
	m_resourceValues[0] = 0; // Matter
	m_resourceValues[1] = 0; // 
	m_resourceValues[2] = 0;
	m_resourceValues[3] = 0;
}
Resources::~Resources()
{

}
void Resources::add(const Resources& other)
{
	for(auto res : other.m_resourceValues)
	{
		initResourceType(res.first);
		m_resourceValues[res.first] += res.second;
	}
}
void Resources::subtract(const Resources& other)
{
	for(auto res : other.m_resourceValues)
	{
		initResourceType(res.first);
		m_resourceValues[res.first] -= res.second;
	}
}

Resources Resources::percentOf(float fraction) const
{
	Resources copy = *this;
	for(auto res : copy.m_resourceValues)
	{
		copy.m_resourceValues[res.first] = static_cast<int>(res.second * fraction);
	}
	return copy;
}
void Resources::intoPacket(sf::Packet* resources) const
{
	sf::Packet& packet = *resources;
	for(auto res : m_resourceValues)
	{
		packet << res.first;
		packet << res.second;
	}
}
void Resources::fromPacket(sf::Packet* resources)
{
	sf::Packet& packet = *resources;
	for(auto res : m_resourceValues)
	{
		ResourceType key;
		ResourceValue value;

		packet >> key;
		packet >> value;

		m_resourceValues[key] = value;
	}
}
bool Resources::hasNegatives() const
{
	for(auto res : m_resourceValues)
	{
		if(res.second < 0)
			return true;
	}
	return false;
}
void Resources::initResourceType(ResourceType type)
{
	bool found = m_resourceValues.find(type) != m_resourceValues.end();
	if(!found)
	{
		m_resourceValues[type] = ResourceValue();
	}
}
void Resources::ChangeIncome(const Resources& amount, Team team)
{
	sf::Packet data;
	int theTeam = (int)team;

	data << theTeam;
	amount.intoPacket(&data);

	Message nwMessage("universe", "changeIncome", data, 0.f, false);
	nwMessage.sendOverNW(false);
	Message::SendUniverse(nwMessage);
}
void Resources::ChangeResources(const Resources& amount, Team team)
{
	sf::Packet data;
	int theTeam = (int)team;

	data << theTeam;
	amount.intoPacket(&data);

	Message nwMessage("universe", "changeResources", data, 0.f, false);
	nwMessage.sendOverNW(false);
	Message::SendUniverse(nwMessage);
}
void Resources::ChangeResourcesFromClient(const Resources& amount, Team team)
{
	sf::Packet data;
	int theTeam = (int)team;

	data << theTeam;
	amount.intoPacket(&data);

	Message nwMessage("universe", "changeResources", data, 0.f, false);
	nwMessage.sendOverNW(true);
	Message::SendUniverse(nwMessage);
}
