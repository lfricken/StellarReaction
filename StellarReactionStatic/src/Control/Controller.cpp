#include "Controller.hpp"

#include "SlaveLocator.hpp"
#include "Chunk.hpp"
#include "CommandInfo.hpp"
#include "Debugging.hpp"

Controller::Controller(const ControllerData& rData) : m_aim(0, 0), m_io(rData.ioComp, &Controller::input, this), m_nw(rData.nwComp, &Controller::pack, &Controller::unpack, this, getGame()->getUniverse().getControllerFactory().getNWFactory())
{
	m_local = false;
	m_slavePosition = -1;

	if(rData.slaveName != "NOSLAVE")
		setSlave(rData.slaveName);
	else
		WARNING;;

	for(int i = 0; i < static_cast<int>(Directive::End); ++i)
	{
		m_directives[static_cast<Directive>(i)] = false;
	}
	for(int i = 1; i <= 9; ++i)
	{
		m_weaponGroups[i] = true;
	}
}
Controller::~Controller()
{

}
void Controller::setSlave(const String& rSlaveName)
{
	m_slaveName = rSlaveName;
	m_slavePosition = getGame()->getUniverse().getChunkPosition(m_slaveName);
	if(m_slavePosition == -1)
		throw new std::runtime_error("Controllers target doesn't exist yet.");
}
const String& Controller::getSlaveName() const
{
	return m_slaveName;
}
IOComponent& Controller::getIOComp()
{
	return m_io;
}
void Controller::setAim(const Vec2& world)//send our aim coordinates
{
	m_nw.toggleNewData(true);
	m_aim = world;
}

const Vec2& Controller::getAim() const
{
	return m_aim;
}
Chunk* Controller::getChunk() const
{
	return getGame()->getUniverse().getChunk(m_slavePosition);
}
/// <summary>
/// Find our slave and set it to aim at a location
/// </summary>
void Controller::processAim() const
{
	auto chunk = getChunk();
	if(chunk != nullptr)
		chunk->setAim(m_aim);
}
/// <summary>
/// returns the value in the slave that's requested
/// </summary>
/// <param name="value">The value.</param>
/// <returns></returns> 
float Controller::get(Request value)//return the requested value
{
	auto chunk = getChunk();
	if(chunk != nullptr)
		return chunk->get(value);
	else
		return 0.f;
}
void Controller::processDirectives()//use our stored directives to send commands
{
	processAim();
	auto targetShip = getChunk();
	if(targetShip != nullptr)
	{
		CommandInfo commands;
		commands.directives = m_directives;
		commands.isLocal = m_local;
		commands.weaponGroups = m_weaponGroups;
		targetShip->directive(commands);
	}
	else
		WARNING;
}
/// <summary>
/// true if this controller is controlled locally (this computer)
/// This is so we can determine whether to ignore outside updates to us
/// and whether to send this data at all
/// </summary>
void Controller::toggleLocal(bool local)
{
	m_local = local;
}
void Controller::locallyUpdate(const CommandInfo& commands)
{
	if(m_local && commands.isLocal)
	{
		m_nw.toggleNewData(true);
		m_directives = commands.directives;
		m_weaponGroups = commands.weaponGroups;
	}
}
void Controller::setPlayerName(const String& rPlayerName)
{
	m_playerName = rPlayerName;
}
const String& Controller::getPlayerName() const
{
	return m_playerName;
}
NetworkComponent& Controller::getNWComp()
{
	return m_nw;
}
void Controller::pack(sf::Packet& rPacket)
{
	int32_t weaponGroupSize = m_weaponGroups.size();

	rPacket << static_cast<float32>(m_aim.x);
	rPacket << static_cast<float32>(m_aim.y);
	for(int32_t i = 0; i < static_cast<int32_t>(Directive::End); ++i)
	{
		rPacket << m_directives[static_cast<Directive>(i)];
	}
	rPacket << weaponGroupSize;
	for(int32_t i = 1; i <= (signed)m_weaponGroups.size(); ++i)
	{
		rPacket << m_weaponGroups[i];
	}
}
void Controller::unpack(sf::Packet& rPacket)
{
	if(getGame()->getNwBoss().getNWState() == NWState::Server)
		m_nw.toggleNewData(true);//if we are the server and we got new data from a client about his control we need to tell the other clients

	/**we need to extract the data no matter what**/
	float32 aimX, aimY;
	Map<Directive, bool> directives;
	int32_t weaponGroupSize;
	Map<int32_t, bool> weaponGroups;

	rPacket >> aimX;
	rPacket >> aimY;

	for(int32_t i = 0; i < static_cast<int32_t>(Directive::End); ++i)
	{
		rPacket >> directives[static_cast<Directive>(i)];
	}
	rPacket >> weaponGroupSize;
	for(int32_t i = 1; i <= weaponGroupSize; ++i)
	{
		rPacket >> weaponGroups[i];
	}

	if(!m_local)//if we are locally controlled, we shouldnt unpack that stuff
	{
		setAim(Vec2(aimX, aimY));
		m_directives = directives;
		m_weaponGroups = weaponGroups;
	}

}
bool Controller::isLocal() const
{
	return m_local;
}
void Controller::input(String rCommand, sf::Packet rData)
{
	sf::Packet data(rData);
	if(rCommand == "setPlayerName")
	{
		String name;
		data >> name;
		setPlayerName(name);
	}
}
