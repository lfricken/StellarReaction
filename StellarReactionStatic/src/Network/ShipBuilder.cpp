#include "ShipBuilder.hpp"
#include "Globals.hpp"
#include "stdafx.hpp"
#include "Controller.hpp"
#include "Chunk.hpp"
#include "Game.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"
#include "Module.hpp"
#include "SlaveLocator.hpp"
#include "BasePlayerTraits.hpp"
#include "Network.hpp"
#include "Player.hpp"
#include "Debugging.hpp"

ShipBuilder::ShipBuilder()
{

}
ShipBuilder::~ShipBuilder()
{

}
void ShipBuilder::Client::shipToGui(const Chunk* ship)
{
	if(ship != NULL)
	{
		auto list = ship->getModules();
		sf::Packet data;
		ShipBuilder::Client::writeToPacket(ship->m_io.getPosition(), list, &data);

		Message ship("ship_editor", "setState", data, 0, false);
		game.getCoreIO().recieve(ship);
	}
	else
		dout << FILELINE;
}
void ShipBuilder::Client::writeToPacket(int targetShipIOPosition, const List<Pair<String, sf::Vector2i> >& modules, sf::Packet* data)
{
	sf::Packet& pack = *data;
	pack << targetShipIOPosition;
	pack << (int32_t)modules.size();

	for(auto it = modules.begin(); it != modules.end(); ++it)
	{
		pack << it->first; //name of module
		pack << it->second.x;//grid position x
		pack << it->second.y;//grid position y
	}
}
void ShipBuilder::Client::readFromPacket(int* targetShipIOPosition, List<Pair<String, sf::Vector2i> >* pModules, sf::Packet data)
{
	auto& modules = *pModules;
	int size;

	data >> *targetShipIOPosition;
	data >> (int32_t)size; modules.resize(size);

	for(auto it = modules.begin(); it != modules.end(); ++it)
	{
		data >> it->first; //name of module
		data >> it->second.x;//grid position x
		data >> it->second.y;//grid position y
	}
}
void ShipBuilder::Client::createChunk(const ChunkDataMessage& data)
{
	sf::Packet messageData;
	data.pack(&messageData);

	Message newChunk("universe", "createChunkCommand", messageData, 0, false);
	Message::SendUniverse(newChunk);
}
String ShipBuilder::Client::getNextSlaveName()
{
	static int counter = 0;
	return "slv_" + String(counter++);
}





void ShipBuilder::Server::rebuild(sf::Packet& rData)
{
	int targetShip;

	rData >> targetShip;

	List<Pair<String, sf::Vector2i> > moduleList;
	extractModules(rData, &moduleList);

	cleanShip(targetShip);
	for(auto it = moduleList.cbegin(); it != moduleList.cend(); ++it)
	{
		attachModule(targetShip, it->first, it->second);
	}
}
void ShipBuilder::Server::attachModule(int targetIOPos, const String& bpName, const sf::Vector2i offset)
{
	sf::Packet pack;
	pack << bpName;
	pack << offset.x;
	pack << offset.y;

	Message attachment((unsigned)targetIOPos, "attachModule", pack, 0, false);
	game.getUniverse().getUniverseIO().recieve(attachment);
}
void ShipBuilder::Server::cleanShip(int targetIOPos)
{
	Message clean((unsigned)targetIOPos, "clear", voidPacket, 0, false);
	game.getUniverse().getUniverseIO().recieve(clean);
}
void ShipBuilder::Server::extractModules(sf::Packet& rData, List<Pair<String, sf::Vector2i> >* list)
{
	String bpName;
	sf::Vector2i pos;
	int num;
	rData >> num;
	for(int i = 0; i < num; ++i)
	{
		rData >> bpName;
		rData >> pos.x;
		rData >> pos.y;
		list->push_back(Pair<String, sf::Vector2i>(bpName, pos));
	}
}
