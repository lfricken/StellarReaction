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

int counter = 0;

ShipBuilder::ShipBuilder()
{

}
ShipBuilder::~ShipBuilder()
{

}
void ShipBuilder::Client::shipToGui(const Chunk* ship)
{
	if(ship != nullptr)
	{
		auto list = ship->getModuleBPs();
		list.insert(list.end(), ship->getStoredModuleBPs().begin(), ship->getStoredModuleBPs().end());
		sf::Packet data;
		data << ship->m_io.getPosition();
		ShipBuilder::Client::writeToPacket(list, &data);

		Message ship("ship_editor", "setState", data, 0, false);
		game.getCoreIO().recieve(ship);
	}
	else
		WARNING;
}
void ShipBuilder::Client::writeToPacket(const List<Pair<String, sf::Vector2i> >& modules, sf::Packet* data)
{
	sf::Packet& pack = *data;
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
String ShipBuilder::Client::getNextSlaveName()
{
	return "slv_" + String(counter++);
}
void ShipBuilder::Client::resetSlaveName()
{
	counter = 0;
}




void ShipBuilder::Server::rebuild(sf::Packet& rData)
{
	int targetShip;


	List<Pair<String, sf::Vector2i> > moduleList;
	Client::readFromPacket(&targetShip, &moduleList, rData);

	cleanShip(targetShip);
	for(auto it = moduleList.cbegin(); it != moduleList.cend(); ++it)
	{
		attachModule(targetShip, it->first, it->second);
	}
	doneBuilding(targetShip);
}
void ShipBuilder::Server::attachModule(int targetIOPos, const String& bpName, const sf::Vector2i offset)
{
	sf::Packet pack;
	pack << bpName;
	pack << offset.x;
	pack << offset.y;

	Message attachment((unsigned)targetIOPos, "attachModule", pack, 0, false);
	Message::SendUniverse(attachment);
}
void ShipBuilder::Server::cleanShip(int targetIOPos)
{
	Message clean((unsigned)targetIOPos, "clear", voidPacket, 0, false);
	Message::SendUniverse(clean);
}
void ShipBuilder::Server::doneBuilding(int targetIOPos)
{
	Message clean((unsigned)targetIOPos, "doneBuilding", voidPacket, 0, false);
	Message::SendUniverse(clean);
}
void ShipBuilder::Server::createChunk(const ChunkDataMessage& data, float delay)
{
	sf::Packet messageData;
	data.pack(&messageData);

	Message newChunk("universe", "createChunkCommand", messageData, delay, false);
	Message::SendUniverse(newChunk);
}
