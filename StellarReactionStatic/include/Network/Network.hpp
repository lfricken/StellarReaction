#pragma once

namespace sf
{
	class Packet;
}

/// <summary>
/// Provides global networking functions.
/// </summary>
class Network
{
public:
	Network();
	~Network();

	/// <summary>
	/// Send Tcp message To Host once only.
	/// </summary>
	static void toHostAtomic(sf::Packet data);
};

