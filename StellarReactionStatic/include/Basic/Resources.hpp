#pragma once

#include "stdafx.hpp"

/// <summary>
/// Stores resources for building things.
/// </summary>
class Resources
{
public:
	Resources();
	~Resources();

	void add(const Resources& other);
	void subtract(const Resources& other);
	Resources percentOf(float fraction) const;
	bool hasNegatives() const;

	void fromPacket(sf::Packet* resources);
	void intoPacket(sf::Packet* resources) const;

	Map<String, int> m_resourceValues;
};

