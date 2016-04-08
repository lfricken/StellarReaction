#pragma once

#include "Globals.hpp"
#include "NetworkComponent.hpp"
#include "IOComponent.hpp"
#include "Universe.hpp"
#include "NonCopyable.hpp"

struct GameObjectData
{
	GameObjectData() : 
		ioComp(&game.getUniverse().getUniverseIO()),
		nwComp()
	{
		pParent = NULL;
	}
	Universe* pParent;
	IOComponentData ioComp;
	NetworkComponentData nwComp;
};

/// <summary>
/// Things owned by Universe, such as Chunks... (what else?)
/// Contains Networking and IO/Eventing
/// pre/post phys update
/// </summary>
class GameObject : NonCopyable
{
public:
	GameObject(const GameObjectData& rData);
	virtual ~GameObject();

	virtual void prePhysUpdate() = 0;//called just before physics step (do extra physics)
	virtual void postPhysUpdate() = 0;//called just after physics step (do corrections, or graphics stuff)

	virtual int getScore() = 0;

	IOComponent m_io;
	NetworkComponent m_nw;

protected:
	virtual void input(std::string rCommand, sf::Packet rData);
	virtual void pack(sf::Packet& rPacket) final;
	virtual void unpack(sf::Packet& rPacket) final;

	Universe& m_rParent;
	int m_score;


private:
};
