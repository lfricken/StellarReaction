#pragma once

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "Globals.hpp"
#include "Universe.hpp"
#include "NetworkComponent.hpp"
#include "Directive.hpp"
#include "Request.hpp"

class Chunk;
struct CommandInfo;

///Data to create a Controller
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


/// A controller sends commands to ships.
/// A local players' commands are sent to a designated controller, who's state is updated between games.
/// Those commands are duplicated between games, and those commands are sent to the targets.
class Controller
{
public:
	Controller(const ControllerData& rData);
	virtual ~Controller();

	/// Set the players name. From some BasePlayerTraits object.
	void setPlayerName(const std::string& rPlayerName);
	/// Designate a name for us to control.
	void setSlave(const std::string& rSlaveName);
	/// Set where we are aiming.
	void setAim(const b2Vec2& world);

	/// Return players name.
	const std::string& getPlayerName() const;
	/// Return name of ship we are controlling.
	const std::string& getSlaveName() const;
	/// Return the actualy ship we are controlling.
	Chunk* getSlave() const;
	/// Find where this controller is aiming.
	const b2Vec2& getAim() const;
	/// Get a value from our ship.
	float get(Request value);
	/// Get pointer to body of ship.
	b2Body* getBodyPtr();
	/// Return the networking object for this controller.
	NetworkComponent& getNWComp();
	/// Return the io object for this controller.
	IOComponent& getIOComp();

	/// Set state of this controller from a local source.
	void locallyUpdate(const CommandInfo& commands);
	/// Use our stored aim to aim our ship.
	void processAim() const;
	/// Use our stored directives to control our ship.
	void processDirectives();
	/// Set wheter this controller accept commands from local sources?
	void toggleLocal(bool local);
	/// Are we a locally controlled controller?
	bool isLocal() const;

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

