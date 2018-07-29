#pragma once

#include "stdafx.hpp"

/// <summary>
/// Can interact with sf::Packets.
/// </summary>
class ISerializable
{
public:

	/// <summary>
	/// Writes the data in the packet into this instance.
	/// </summary>
	virtual void ISerializable::fromPacket(sf::Packet* data) = 0;

	/// <summary>
	/// Puts this data into the given packet.
	/// </summary>
	virtual void ISerializable::intoPacket(sf::Packet* data) const = 0;
};

