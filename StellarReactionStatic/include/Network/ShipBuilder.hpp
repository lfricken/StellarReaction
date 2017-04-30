#pragma once
#include "stdafx.hpp"

class BasePlayerTraits;
class Chunk;
struct ChunkDataMessage;
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
		/// <summary>
		/// Returns the next name for a slave.
		/// </summary>
		static String getNextSlaveName();
		static void resetSlaveName();

		static void writeToPacket(int targetShipIOPosition, const List<Pair<String, sf::Vector2i> >& modules, sf::Packet* data);
		static void readFromPacket(int* targetShipIOPosition, List<Pair<String, sf::Vector2i> >* modules, sf::Packet data);

		static void createChunk(const ChunkDataMessage& data);
	};



	/// <summary>
	/// Only called by Host
	/// </summary>
	class Server
	{
	public:
		/// <summary>
		/// Build a ship using the writeToPacket return as input.
		/// </summary>
		static void rebuild(sf::Packet& rData);
	private:
		static void attachModule(int targetIOPos, const String& bpName, const sf::Vector2i offset);
		static void cleanShip(int targetIOPos);
		static void doneBuilding(int targetIOPos);
	};



	List<std::pair<String, sf::Vector2i> > getModules();
};

