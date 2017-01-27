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


ShipBuilder::ShipBuilder()
{

}
ShipBuilder::~ShipBuilder()
{

}
bool ShipBuilder::handleCommand(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom)
{
	if(rCommand == "rebuild")//a player wants to attach a module to thier ship, from their inventory
		rebuild(rCommand, rData, pFrom);
	else if(rCommand == "buyModule")
		Gui::buyModule(rCommand, rData, pFrom);
	else if(rCommand == "addModule")
		Gui::addModule(rCommand, rData, pFrom);
	else
		return false;
	return true;
}
void ShipBuilder::rebuild(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom)
{
	Controller* pCon = &game.getUniverse().getControllerFactory().getController(pFrom->getController());
	String slaveName = pCon->getSlaveName();
	Chunk* ship = game.getUniverse().getSlaveLocator().find(slaveName);

	List<Pair<String, sf::Vector2i> > moduleList;
	extractModules(rData, &moduleList);

	cleanShip(ship);
	for(auto it = moduleList.cbegin(); it != moduleList.cend(); ++it)
	{
		attachModule(ship, it->first, it->second);
	}
}
void ShipBuilder::Gui::buyModule(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom)
{
	String bpName;
	rData >> bpName;

	const ModuleData* pMod = game.getUniverse().getBlueprints().getModuleSPtr(bpName).get();
	if(pMod != NULL)
	{
		Money cost = pMod->cost;//see if we can afford it
		if(pFrom->getMoney() >= cost)
		{
			pFrom->addModule(bpName, sf::Vector2i(0, 0));//tell whoever this command came from to add a module to their gui
			pFrom->changeMoney(-cost);
		}
	}
	else
		Print << FILELINE;
}
void ShipBuilder::Gui::addModule(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom)
{
	String bpName;
	sf::Vector2i pos;
	rData >> bpName;
	rData >> pos.x;
	rData >> pos.y;
	const ModuleData* pMod = game.getUniverse().getBlueprints().getModuleSPtr(bpName).get();
	if(pMod != NULL)
	{
		pFrom->addModule(bpName, pos);//tell whoever this command came from to add a module to their gui
		//just assume they already paid for it, or aquired it otherwise
	}
	else
		Print << FILELINE;
}
void ShipBuilder::cleanShip(const Chunk* ship)
{
	int targetPos = ship->m_io.getPosition();

	Message clean((unsigned)targetPos, "clear", voidPacket, 0, false);
	clean.sendOverNW(true);
	game.getUniverse().getUniverseIO().recieve(clean);
}
void ShipBuilder::attachModule(Chunk* ship, const String& bpName, const sf::Vector2i offset)
{
	sf::Packet pack;
	pack << bpName;
	pack << offset.x;
	pack << offset.y;
	int targetPos = ship->m_io.getPosition();

	Message attachment((unsigned)targetPos, "attachModule", pack, 0, false);
	attachment.sendOverNW(true);
	game.getUniverse().getUniverseIO().recieve(attachment);
}
void ShipBuilder::extractModules(sf::Packet& rData, List<Pair<String, sf::Vector2i> >* list)
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
void ShipBuilder::insertModules(sf::Packet& rData, const List<Pair<String, sf::Vector2i> >& list)
{
	//for(auto it = list.cbegin(); it != list.cend(); ++it)
	//{
	//	sf::Packet pack;
	//	pack << bpName;
	//	pack << x;
	//	pack << y;

	//	Message attach((unsigned)targetPos, "attachModule", pack, 0, false);
	//	attach.sendOverNW(true);
	//	game.getUniverse().getUniverseIO().recieve(attach);
	//}

}
