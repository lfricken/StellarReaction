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
	bool hasNegatives() const;

	void outOf(sf::Packet* resources);
	void into(sf::Packet* resources) const;

	Map<String, int> m_resourceValues;
};

