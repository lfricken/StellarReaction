#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Globals.hpp"
#include "NetworkComponent.hpp"
#include "IOComponent.hpp"
#include "Universe.hpp"

struct GameObjectData
{
	GameObjectData() :
		ioComp(game.getUniverse().getUniverseIO()),
		nwComp()
	{

	}
	IOComponentData ioComp;
	NetworkComponentData nwComp;
};

/// <summary>
/// Things owned by Universe, such as Chunks... (what else?)
/// Contains Networking and IO/Eventing
/// pre/post phys update
/// </summary>
class GameObject
{
public:
	GameObject(const GameObjectData& rData);
	virtual ~GameObject();

	virtual void prePhysUpdate() = 0;//called just before physics step (do extra physics)
	virtual void postPhysUpdate() = 0;//called just after physics step (do corrections, or graphics stuff)

	IOComponent m_io;
	NetworkComponent m_nw;

protected:
	virtual void input(std::string rCommand, sf::Packet rData);
	virtual void pack(sf::Packet& rPacket) final;
	virtual void unpack(sf::Packet& rPacket) final;


private:
};

#endif // GAMEOBJECT_HPP
