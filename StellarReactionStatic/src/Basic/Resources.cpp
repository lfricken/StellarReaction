#include "Resources.hpp"


Resources::Resources()
{
	m_resourceValues["A"] = 0;
	m_resourceValues["B"] = 0;
	m_resourceValues["C"] = 0;
}
Resources::~Resources()
{

}
void Resources::add(const Resources& other)
{
	for(auto it = other.m_resourceValues.cbegin(); it != other.m_resourceValues.cend(); ++it)
	{
		m_resourceValues[it->first] += it->second;
	}
}
void Resources::subtract(const Resources& other)
{
	for(auto it = other.m_resourceValues.cbegin(); it != other.m_resourceValues.cend(); ++it)
	{
		m_resourceValues[it->first] -= it->second;
	}
}

Resources Resources::percentOf(float fraction) const
{
	Resources copy = *this;
	for(auto it = copy.m_resourceValues.begin(); it != copy.m_resourceValues.end(); ++it)
	{
		it->second = static_cast<int>(it->second * fraction);
	}
	return copy;
}
void Resources::intoPacket(sf::Packet* resources) const
{
	sf::Packet& res = *resources;
	for(auto it = m_resourceValues.cbegin(); it != m_resourceValues.cend(); ++it)
	{
		res << (int32_t)it->second;
	}
}
void Resources::fromPacket(sf::Packet* resources)
{
	sf::Packet& res = *resources;
	for(auto it = m_resourceValues.cbegin(); it != m_resourceValues.cend(); ++it)
	{
		int32_t val = 0;
		res >> val;
		m_resourceValues[it->first] = val;
	}
}
bool Resources::hasNegatives() const
{
	for(auto it = m_resourceValues.cbegin(); it != m_resourceValues.cend(); ++it)
	{
		if(it->second < 0)
			return true;
	}
	return false;
}
