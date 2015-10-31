#ifndef NETWORKBOSS_HPP
#define NETWORKBOSS_HPP

#include "stdafx.hpp"
#include "Globals.hpp"
#include "Connection.hpp"
#include "IOComponent.hpp"
#include "NetworkFactory.hpp"

class PlayerState;

enum class NWState
{
	Local,
	Client,
	Server,
};

struct NetworkBossData
{
	NetworkBossData() :
		ioComp(game.getCoreIO())
	{
		ioComp.name = "networkboss";
	}

	IOComponentData ioComp;
};

class NetworkBoss
{
public:
	NetworkBoss(const NetworkBossData& rData);
	virtual ~NetworkBoss();


	/**UTILITY**/
	NetworkFactory& getNWFactory();
	NetworkFactory& getNWFactoryTcp();

	void messageLobbyLocal(const std::string& rMessage);
	void messageLobby(const std::string& rMessage);
	void messagePlayerCount();


	bool setRecievePort(unsigned short port);//set receiving port, returns whether the bind was successful
	bool hasConnections();//are we connected to anyone?
	Connection* findConnection(const sf::IpAddress& rAdd, unsigned short fromUDPPort);
	void addConnection(sptr<sf::TcpSocket> spTcpSocket, bool valid);//add this connection to our clients list and handshake it a few times (SERVER ONLY)

	bool gameHasStarted() const;
	bool isClient() const;
	NWState getNWState() const;



	/**CHANGE STATE**/
	void setClient(const std::string& address, unsigned short port, float timeout);//set us to client mode and connect to a host
	void setLocal();//set us to local mode and ignore nw activity
	void setServer(unsigned short port, float timeout);//set us to host mode and listen for connections



	/**UPDATE**/
	void update();



	/**REDUCTION**/
	void playerOption(sf::Packet& rData, BasePlayerTraits* pFrom);
	void recieveLevel(sf::Packet& data);
	void launchMultiplayerGame();

protected:
	void input(const std::string rCommand, sf::Packet rData);

	void udpRecieve();//receive from (udp)
	void tcpRecieve();//receive data from each TcpPort (tcp)

	void sendUdp();//send game data (udp)
	void sendTcp();//send things that can't get lost (tcp)

	void tcpListen();//check for new connections
	void updateConnections();//delete connections if they go bad

	void setState(NWState state, bool open, bool acceptsLocal, bool hideLobby, bool hideConnectScreen);
private:
	IOComponent m_io;

	std::string m_remoteIP;//the address we will attempt to join if a client
	unsigned short m_port;//the port we will try to join if a client, or that we will host on if a client
	float m_timeOut;//timeout we use for connections

	bool m_nwGameStarted;//has the game started for us yet?
	bool m_isOpen;//are we accepting connections
	NWState m_state;//are we Client, Server, Local

	sf::TcpListener m_listener;
	sf::UdpSocket m_udp;
	std::vector<sptr<Connection> > m_connections;

	NetworkFactory m_nwFactoryTcp;
	NetworkFactory m_nwFactory;
};

#endif // NETWORKBOSS_HPP
