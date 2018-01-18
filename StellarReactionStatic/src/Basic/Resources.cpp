#include "Resources.hpp"


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
