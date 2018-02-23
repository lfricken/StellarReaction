#pragma once

#include "stdafx.hpp"

enum class Team;
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

	/// <summary>
	/// Adds the amount to this instance.
	/// </summary>
	void add(const Resources& other);
	/// <summary>
	/// Subtracts the amount from this instance.
	/// </summary>
	void subtract(const Resources& other);
	Resources percentOf(float fraction) const;
	bool hasNegatives() const;

	void fromPacket(sf::Packet* resources);
	void intoPacket(sf::Packet* resources) const;

	Map<ResourceType, ResourceValue> m_resourceValues;

	enum ResourceDefinitions
	{
		Matter,
		Energy,
		Perception,
	};

	static void ChangeIncome(const Resources& amount, Team team);
	static void ChangeResources(const Resources& amount, Team team);
	static void ChangeResourcesFromClient(const Resources& amount, Team team);


private:
	/// <summary>
	/// Guarantee that a type of resource is initialized.
	/// </summary>
	void initResourceType(ResourceType type);
};

