#pragma once

#include "stdafx.hpp"

/// <summary>
/// The variable type used to record amounts of a resource.
/// </summary>
typedef int ResourceValue;

/// <summary>
/// The type of resource (iron, nickel, etc.)
/// </summary>
typedef int ResourceType;

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

	Map<ResourceType, ResourceValue> m_resourceValues;

private:
	/// <summary>
	/// Guarantee that a type of resource is initialized.
	/// </summary>
	void initResourceType(ResourceType type);
};

