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
		buyModule(rCommand, rData, pFrom);
	else if(rCommand == "addModule")
		addModule(rCommand, rData, pFrom);
	else
		return false;
	return true;
}
void ShipBuilder::rebuild(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom)
{
	String bpName;
	float x;
	float y;
	List<std::pair<String, sf::Vector2f> > modules;

	Controller* pCon = &game.getUniverse().getControllerFactory().getController(pFrom->getController());
	String slaveName = pCon->getSlaveName();
	Chunk* pTemp = game.getUniverse().getSlaveLocator().find(slaveName);
	int targetPos = pTemp->m_io.getPosition();

	Message clean((unsigned)targetPos, "clear", voidPacket, 0, false);
	clean.sendOverNW(true);
	game.getUniverse().getUniverseIO().recieve(clean);

	List<std::pair<String, Vec2> > available = pFrom->getOwnedModuleTitles();

	int num;
	rData >> num;
	for(int i = 0; i < num; ++i)
	{
		rData >> bpName;
		rData >> x;
		rData >> y;

		bool canAdd = false;
		//can we add this module?
		for(auto it = available.begin(); it != available.end(); ++it)
		{
			if(it->first == bpName)
			{
				canAdd = true;
				available.erase(it);
				break;
			}
		}

		if(canAdd)//if the user had that in their inventory
		{//then we just removed it, and we should add it to thier ship
			sf::Packet pack;
			pack << bpName;
			pack << x;
			pack << y;

			Message attach((unsigned)targetPos, "attachModule", pack, 0, false);
			attach.sendOverNW(true);
			game.getUniverse().getUniverseIO().recieve(attach);
		}
	}
}
void ShipBuilder::buyModule(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom)
{
	String bpName;
	rData >> bpName;

	const ModuleData* pMod = game.getUniverse().getBlueprints().getModuleSPtr(bpName).get();
	if(pMod != NULL)
	{
		Money cost = pMod->cost;//see if we can afford it
		if(pFrom->getMoney() >= cost)
		{
			pFrom->addModule(bpName, Vec2(0, 0));//tell whoever this command came from to add a module to their gui
			pFrom->changeMoney(-cost);
		}
	}
	else
		Print << FILELINE;
}
void ShipBuilder::addModule(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom)
{

	String bpName;
	float x = 0;
	float y = 0;
	rData >> bpName;
	rData >> x;
	rData >> y;
	const ModuleData* pMod = game.getUniverse().getBlueprints().getModuleSPtr(bpName).get();
	if(pMod != NULL)
	{
		pFrom->addModule(bpName, Vec2(x, y));//tell whoever this command came from to add a module to their gui
		//just assume they already paid for it, or aquired it otherwise
	}
	else
		Print << FILELINE;
}

