#ifndef NETWORKBOSS_HPP
#define NETWORKBOSS_HPP

#include "stdafx.hpp"
#include "Globals.hpp"
#include "Connection.hpp"
#include "IOComponent.hpp"
#include "NetworkFactory.hpp"
#include "NonCopyable.hpp"
#include "ShipBuilder.hpp"

class PlayerState;


/// \brief What state is the NetworkBoss (us) in?
enum class NWState
{
	Local,
	Client,
	Server,
};
/// \brief Data to initialize NetworkBoss
struct NetworkBossData
{
	NetworkBossData() :
		ioComp(&getGame()->getCoreIO())
	{
		ioComp.name = "networkboss";//how to send io messages to network boss
	}

	IOComponentData ioComp;
};
/// \brief Manages network Connections, and sending and recieving data.
class NetworkBoss : NonCopyable
{
public:
	NetworkBoss(const NetworkBossData& rData);
	virtual ~NetworkBoss();


	/// Returns thet NetworkFactory that syncs via udp.
	NetworkFactory& getNWDataFactory();
	/// Returns thet NetworkFactory that syncs via tcp.
	NetworkFactory& getNWFactoryTcp();

	/// Message only the local lobby.
	void messageLobbyLocal(const String& rMessage);
	/// Send message globaly (to everyone in the lobby).
	void messageLobby(const String& rMessage);
	/// Send global message about number of players in the lobby.
	void messagePlayerCount();

	/// Set receiving port. Returns whether the bind was successful.
	bool setRecievePort(unsigned short port);
	/// Returns whether we are in a lobby with anyone.
	bool hasConnections();
	/// Finds a particular Connection based on the address and the from port (udp).
	Connection* findConnection(const sf::IpAddress& rAdd, unsigned short fromUDPPort);
	/// Add this Connection to our Clients list and handshake it a few times (called by Server only).
	void addConnection(sptr<sf::TcpSocket> spTcpSocket, bool valid);

	/// Returns whether the game has already started.
	bool gameHasStarted() const;
	/// Returns whether we are a Client, as opposed to the Server.
	bool isClient() const;
	/// Return current state of network via NWState.
	NWState getNWState() const;



	/// Set us to a Client, and connect to a Server.
	void setClient(const String& address, unsigned short port, float timeout);
	/// Disconnect us from any network activity.
	void setLocal();
	/// Set us to a Server, and listen for Clients.
	void setServer(unsigned short port, float timeout);

	/// Return list of connections.
	List<sptr<Connection> >& getConnections();

	/// Listen for new Connection's, update Connection's, recieve data, send data.
	void update();



	/**REDUCTION**/

	/// Called by Server when receiving a player option, such as changing teams or ships, buying modules, etc.
	void playerOption(sf::Packet rData, BasePlayerTraits* pFrom);
	/// We are being told to start the getGame()-> data contains the game information.
	void recieveLevel(sf::Packet& data);
	/// Called by Server when player clicks Launch. Sends data to Clients.
	void launchMultiplayerGame();

protected:
	void input(const String rCommand, sf::Packet rData);

	/// Receive (udp).
	void udpRecieve();
	/// Receive (tcp).
	void tcpRecieve();

	/// Send real time game data (udp). Info that needs to be delivered fast.
	void sendUdp();
	/// Send data that can't get lost (tcp packets). Slower than udp.
	void sendTcp();

	/// Check for new connections.
	void tcpListen();
	/// Delete connections if they go bad (timeout or disconnect).
	void updateConnections();

	void setState(NWState state, bool open, bool acceptsLocal, bool hideLobby, bool hideConnectScreen);
private:
	IOComponent m_io;

	String m_remoteIP;//the address we will attempt to join if a client
	unsigned short m_port;//the port we will try to join if a client, or that we will host on if a client
	float m_timeOut;//timeout we use for connections

	bool m_nwGameStarted;//has the game started for us yet?
	bool m_isOpen;//are we accepting connections
	NWState m_state;//are we Client, Server, Local
	int m_numAI;

	sf::TcpListener m_listener;
	sf::UdpSocket m_udp;
	//each connection is another player (on server)
	//does not store the hosts player
	List<sptr<Connection> > m_connections;

	NetworkFactory m_nwFactoryTcp;
	/// <summary>
	/// ships, modules
	/// </summary>
	NetworkFactory m_dataProtocol;
};

#endif // NETWORKBOSS_HPP
