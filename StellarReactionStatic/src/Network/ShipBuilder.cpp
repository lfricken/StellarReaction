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
void ShipBuilder::shipToGui(const Chunk* ship)
{
	if(ship != nullptr)
	{
		auto list = ship->getModuleBPs();
		list.insert(list.end(), ship->getStoredModuleBPs().begin(), ship->getStoredModuleBPs().end());
		sf::Packet data;
		data << ship->m_io.getPosition();
		ShipBuilder::writeToPacket(list, &data);

		Message ship("ship_editor", "setState", data, 0, false);
		getGame()->getCoreIO().recieve(ship);
	}
	else
		WARNING;
}
void ShipBuilder::writeToPacket(const List<Pair<String, sf::Vector2i> >& modules, sf::Packet* data)
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
void ShipBuilder::readFromPacket(int* targetShipIOPosition, List<Pair<String, sf::Vector2i> >* pModules, sf::Packet data)
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
String ShipBuilder::getNextSlaveName()
{
	return "slv_" + String(counter++);
}
void ShipBuilder::resetSlaveName()
{
	counter = 0;
}


void ShipBuilder::Server::createChunk(const ChunkDataMessage& data, float delay)
{
	if(getGame()->getNwBoss().getNWState() == NWState::Server)
		ShipBuilder::Networked::createChunkFromClient(data, delay);
}


void ShipBuilder::Networked::createChunkFromClient(const ChunkDataMessage& data, float delay)
{
	sf::Packet messageData;
	data.pack(&messageData);

	Message newChunk("universe", "createChunkCommand", messageData, delay, false);
	newChunk.sendOverNW(true);
	Message::SendUniverse(newChunk);
}
void ShipBuilder::Networked::rebuildFromClient(int targetShip, const List<Pair<String, sf::Vector2i> >& moduleList)
{
	cleanShip(targetShip);
	for(auto it = moduleList.cbegin(); it != moduleList.cend(); ++it)
	{
		attachModule(targetShip, it->first, it->second);
	}
	doneBuilding(targetShip);
}
void ShipBuilder::Networked::attachModule(int targetIOPos, const String& bpName, const sf::Vector2i offset)
{
	sf::Packet pack;
	pack << bpName;
	pack << offset.x;
	pack << offset.y;

	Message attachment((unsigned)targetIOPos, "attachModule", pack, 0, false);
	attachment.sendOverNW(true);
	Message::SendUniverse(attachment);
}
void ShipBuilder::Networked::cleanShip(int targetIOPos)
{
	Message clean((unsigned)targetIOPos, "clear", voidPacket, 0, false);
	clean.sendOverNW(true);
	Message::SendUniverse(clean);
}
void ShipBuilder::Networked::doneBuilding(int targetIOPos)
{
	Message clean((unsigned)targetIOPos, "doneBuilding", voidPacket, 0, false);
	clean.sendOverNW(true);
	Message::SendUniverse(clean);
}
