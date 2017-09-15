#pragma once
#include "stdafx.hpp"

class BasePlayerTraits;
class Chunk;
struct ChunkDataMessage;
/// <summary>
/// Handles ship manipulation messages.
/// </summary>
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
		/// <summary>
		/// Resets the counter for generating slave names.
		/// </summary>
		static void resetSlaveName();

		/// <summary>
		/// Given a target, module list, write to a packet.
		/// </summary>
		static void writeToPacket(int targetShipIOPosition, const List<Pair<String, sf::Vector2i> >& modules, sf::Packet* data);
		/// <summary>
		/// Get a target and modules out of a data packet.
		/// </summary>
		static void readFromPacket(int* targetShipIOPosition, List<Pair<String, sf::Vector2i> >* modules, sf::Packet data);
	};



	/// <summary>
	/// Functions that produce Universe messages that only the server processes.
	/// </summary>
	class Server
	{
	public:
		/// <summary>
		/// Build a ship using the writeToPacket return as input.
		/// </summary>
		static void rebuild(sf::Packet& rData);
		/// <summary>
		/// Create a chunk in the game from a chunk data message.
		/// </summary>
		static void createChunk(const ChunkDataMessage& data, float delay);
	private:
		/// <summary>
		/// Attach a module blueprint to the target ship at the specified position.
		/// </summary>
		static void attachModule(int targetIOPos, const String& bpName, const sf::Vector2i offset);
		/// <summary>
		/// Removes all modules from the ship.
		/// </summary>
		static void cleanShip(int targetIOPos);
		/// <summary>
		/// Lets the chunk know it is done being built so it can do some after processing.
		/// </summary>
		static void doneBuilding(int targetIOPos);
	};

};

