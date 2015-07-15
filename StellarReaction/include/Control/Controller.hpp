#ifndef INTELLIGENCE_HPP
#define INTELLIGENCE_HPP

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "Globals.hpp"
#include "Universe.hpp"
#include "NetworkComponent.hpp"

class Slave;

enum class Directive
{
	/**== MOVEMENT ==**/
	Up,
	Down,
	Left,
	Right,
	RollCCW,//roll left
	RollCW,//roll right


	/**== COMBAT ==**/
	FirePrimary,
	FireSecondary,

	NextWeapon,
	PrevWeapon,
	Reload,

	/**== MISC. ==**/
	Use,
	ShieldToggle,
	Special1,
	Special2,
	End,
};
enum class Request
{
	Zoom,
	MaxZoom,

	Energy,
	MaxEnergy,

	Ballistics,
	MaxBallistics,

	Missiles,
	MaxMissiles,
};


struct ControllerData
{
	ControllerData() :
		nwComp(),
		ioComp(game.getCoreIO()),
		slaveName("NOSLAVE")
	{
	}

	NetworkComponentData nwComp;
	IOComponentData ioComp;
	std::string slaveName;
};


class Controller
{
public:
	Controller(const ControllerData& rData);
	virtual ~Controller();

	/**== SETTERS ==**/
	void setPlayerName(const std::string& rPlayerName);
	void setSlave(const std::string& rSlaveName);
	void setAim(const b2Vec2& world);//send our aim coordinates
	void updateDirectives(const std::map<Directive, bool>& rDirs);
	void directive(Directive issue) const;//send a command to our slave
	void processAim() const;
	void processDirectives();//use our stored directives to send commands
	void toggleLocal(bool local);

	/**GETTERS**/
	const std::string& getPlayerName() const;
	const std::string& getSlaveName() const;
	const b2Vec2& getAim() const;
	float get(Request value);//return the requested value
	b2Body* getBodyPtr();//return our chunk body if we have one
	NetworkComponent& getNWComp();
	IOComponent& getIOComp();

protected:
	virtual void input(std::string rCommand, sf::Packet rData);
	virtual void pack(sf::Packet& rPacket) final;
	virtual void unpack(sf::Packet& rPacket) final;

	int m_slavePosition;//position of our slave in the list
	std::string m_slaveName;//name of the game thing we sends commands to
	b2Vec2 m_aim;//where we are aiming in the world ATM
	std::map<Directive, bool> m_directives;//up, down, rollCW, roll CCW, ect.

	NetworkComponent m_nw;

private:
	bool m_local;//true if this is controlled by a local player and shouldn't be unpacked(NW) into
	IOComponent m_io;
	std::string m_playerName;//name of us in game, like BobbyLolcatz99, not used for anything but player reading
};

#endif // INTELLIGENCE_HPP
