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
bool ShipBuilder::handleCommand(const String& rCommand, sf::Packet rData, BasePlayerTraits* pFrom)
{
	//if(rCommand == "rebuild")//a player wants to attach a module to thier ship, from their inventory
	//	
	//else if(rCommand == "buyModule")
	//	Client::buyModule(rData);
	////else if(rCommand == "addModule")
	////	Server::addModule(rCommand, rData, pFrom);
	//else
	//	return false;
	return true;
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
void ShipBuilder::Client::addModuleToGui(const String& newTitle, const sf::Vector2i& rPos)
{
	//m_owned.push_back(pair<String, Vec2>(newTitle, rPos));
	sf::Packet pack;
	pack << newTitle;
	pack << rPos.x;
	pack << rPos.y;
	Message modAdded("ship_editor", "addModuleToGui", pack, 0.f, false);
	game.getCoreIO().recieve(modAdded);
}


//void ShipBuilder::Server::addModule(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom)
//{
//	String bpName;
//	sf::Vector2i pos;
//	rData >> bpName;
//	rData >> pos.x;
//	rData >> pos.y;
//	const ModuleData* pMod = game.getUniverse().getBlueprints().getModuleSPtr(bpName).get();
//	if(pMod != nullptr)
//	{
//		pFrom->addModule(bpName, pos);//tell whoever this command came from to add a module to their gui
//		//just assume they already paid for it, or aquired it otherwise
//	}
//	else
//		Print << FILELINE;
//}
void ShipBuilder::Client::buyModule(sf::Packet& rData)
{
	BasePlayerTraits& buyer = game.getLocalPlayer();

	String bpName;
	sf::Vector2i pos;
	rData >> bpName;
	rData >> pos.x;
	rData >> pos.y;

	const ModuleData* module = game.getUniverse().getBlueprints().getModuleSPtr(bpName).get();
	if(module != nullptr)
	{
		Money cost = module->cost;//see if we can afford it
		if(buyer.getMoney() >= cost)
		{
			Client::addModuleToGui(bpName, pos);//tell whoever this command came from to add a module to their gui
			buyer.changeMoney(-cost);
		}
	}
	else
		Print << FILELINE;
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
	attachment.sendOverNW(true);
	game.getUniverse().getUniverseIO().recieve(attachment);
}
void ShipBuilder::Server::cleanShip(int targetIOPos)
{
	Message clean((unsigned)targetIOPos, "clear", voidPacket, 0, false);
	clean.sendOverNW(true);
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
