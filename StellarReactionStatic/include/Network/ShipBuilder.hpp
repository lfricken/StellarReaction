#pragma once
#include "stdafx.hpp"

class BasePlayerTraits;
class Chunk;
/// \brief Handles ship manipulation messages.
class ShipBuilder
{
public:
	ShipBuilder();
	~ShipBuilder();

	/// <summary>
	/// Handle a command being recieved by either a client or host.
	/// </summary>
	static bool handleCommand(const String& rCommand, sf::Packet rData, BasePlayerTraits* pFrom);

	/// <summary>
	/// Called only on the individual client.
	/// </summary>
	class Client
	{
	public:
		/// <summary>
		/// Takes a chunk and sets the local editor to edit this.
		/// </summary>
		static void shipToGui(const Chunk* ship);
		static void writeToPacket(int targetIOpos, const List<Pair<String, sf::Vector2i> >& modules, sf::Packet* data);
		static void readFromPacket(int* targetIOpos, List<Pair<String, sf::Vector2i> >* modules, sf::Packet data);

	};



	/// <summary>
	/// Only called by Host
	/// </summary>
	class Server
	{
	public:
		static void addModule(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);
		static void buyModule(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);

		static void rebuild(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);
		static void attachModule(Chunk* ship, const String& bpName, const sf::Vector2i offset);
		static void cleanShip(const Chunk* ship);
		static void extractModules(sf::Packet& rData, List<Pair<String, sf::Vector2i> >* list);
	};



	List<std::pair<String, sf::Vector2i> > getModules();
};

