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
	/// Called only on the individual client.
	/// </summary>
	class Client
	{
	public:
		/// <summary>
		/// Takes a chunk and sets the local editor to edit this.
		/// </summary>
		static void shipToGui(const Chunk* ship);

		static void writeToPacket(int targetShipIOPosition, const List<Pair<String, sf::Vector2i> >& modules, sf::Packet* data);
		static void readFromPacket(int* targetShipIOPosition, List<Pair<String, sf::Vector2i> >* modules, sf::Packet data);

	};



	/// <summary>
	/// Only called by Host
	/// </summary>
	class Server
	{
	public:
		static void rebuild(sf::Packet& rData);
	private:
		static void attachModule(int targetShipIOPosition, const String& bpName, const sf::Vector2i offset);
		static void cleanShip(int targetShipIOPosition);
		static void extractModules(sf::Packet& rData, List<Pair<String, sf::Vector2i> >* list);
	};



	List<std::pair<String, sf::Vector2i> > getModules();
};

