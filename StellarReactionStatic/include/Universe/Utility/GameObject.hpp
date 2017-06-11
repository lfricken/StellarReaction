#pragma once

#include "Globals.hpp"
#include "NetworkComponent.hpp"
#include "IOComponent.hpp"
#include "Universe.hpp"
#include "NonCopyable.hpp"

/// Blueprint for GameObject.
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

/// Base object to allow networking and IO interaction.
class GameObject : NonCopyable
{
public:
	GameObject(const GameObjectData& rData);
	virtual ~GameObject();
	///Pure virtual function to process actions on object before performing physics updates.
	virtual void prePhysUpdate() = 0;
	///Pure virtual function to process actions on object after performing physics updates.
	virtual void postPhysUpdate() = 0;
	///Pure virtual function to return the score of the game object.
	virtual int getScore() = 0;

	IOComponent m_io;
	NetworkComponent m_nw;

	/// <summary>
	/// Position of this element in the list of game objects.
	/// </summary>
	int universePosition;
protected:
	virtual void input(String rCommand, sf::Packet rData);
	virtual void pack(sf::Packet& rPacket) final;
	virtual void unpack(sf::Packet& rPacket) final;

	Universe& m_rParent;
	int m_score;


private:
};
