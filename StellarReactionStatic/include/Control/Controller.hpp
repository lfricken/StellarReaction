#ifndef INTELLIGENCE_HPP
#define INTELLIGENCE_HPP

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "Globals.hpp"
#include "Universe.hpp"
#include "NetworkComponent.hpp"
#include "Directive.hpp"
#include "Request.hpp"

class Chunk;
struct CommandInfo;

struct ControllerData
{
	ControllerData() :
		nwComp(),
		ioComp(&game.getCoreIO()),
		slaveName("NOSLAVE")
	{
	}

	NetworkComponentData nwComp;
	IOComponentData ioComp;
	std::string slaveName;
};

/// <summary>
/// A controller sends commands to ships.
/// A local players commands are sent to a designated controller, who's state is updated between games.
/// Chunks are then also synced
/// </summary>
class Controller
{
public:
	Controller(const ControllerData& rData);
	virtual ~Controller();

	/**== SETTERS ==**/
	void setPlayerName(const std::string& rPlayerName);
	void setSlave(const std::string& rSlaveName);//designate a name for us to control
	void setAim(const b2Vec2& world);//send our aim coordinates

	/**GETTERS**/
	const std::string& getPlayerName() const;
	const std::string& getSlaveName() const;//what's the name of the ship are we controlling
	Chunk* getSlave() const;//get the pointer to the ship this controller controls
	const b2Vec2& getAim() const;//where is this controller aiming?
	float get(Request value);//gives feedback from the thing we are controlling
	b2Body* getBodyPtr();//return our chunk body if we have one
	NetworkComponent& getNWComp();
	IOComponent& getIOComp();

	void locallyUpdate(const CommandInfo& commands);//set our state
	void processAim() const;//use our stored aim to send commands
	void processDirectives();//use our stored directives to send commands
	void toggleLocal(bool local);//true or false whether this controller is locally controlled
	bool isLocal() const;//are we a locally controlled controller?

protected:
	virtual void input(std::string rCommand, sf::Packet rData);
	virtual void pack(sf::Packet& rPacket) final;
	virtual void unpack(sf::Packet& rPacket) final;

	int m_slavePosition;//position of our slave in the list
	std::string m_slaveName;//name of the game thing we sends commands to
	b2Vec2 m_aim;//where we are aiming in the world ATM
	std::map<Directive, bool> m_directives;//up, down, rollCW, roll CCW, ect.
	Map<int, bool> m_weaponGroups;//decides which weapon groups are on or off

	NetworkComponent m_nw;

private:
	bool m_local;//true if this is controlled by a local player and shouldn't be unpacked(NW) into
	IOComponent m_io;
	std::string m_playerName;//name of us in game, like BobbyLolcatz99, not used for anything but player reading
};

#endif // INTELLIGENCE_HPP
