#include "NetworkBoss.hpp"
#include "Protocol.hpp"
#include "IOManager.hpp"
#include "Universe.hpp"
#include "Player.hpp"

using namespace std;
using namespace sf;

NetworkBoss::NetworkBoss(const NetworkBossData& rData) : m_io(rData.ioComp, &NetworkBoss::input, this), m_nwFactory("standard"), m_nwFactoryTcp("tcp")
{
	m_state = NWState::Local;
	m_udp.unbind();
	m_udp.setBlocking(false);

	m_joinIP = "127.0.0.1";
	m_joinPort = 5050;//port used for TCP, m_joinPort+1 used for udp

	m_timeOut = 10.f;//how long the timeout is
	m_nwGameStarted = false;
	m_isOpen = false;//are we accepting connections

	m_listener.setBlocking(false);
}
NetworkBoss::~NetworkBoss()
{
	m_udp.unbind();
}




/**UTILITY**/
bool NetworkBoss::gameHasStarted() const
{
	return m_nwGameStarted;
}
NetworkFactory& NetworkBoss::getNWFactory()
{
	return m_nwFactory;
}
NetworkFactory& NetworkBoss::getNWFactoryTcp()
{
	return m_nwFactoryTcp;
}

bool NetworkBoss::setRecievePort(unsigned short port)//set receiving port, returns whether the bind was successful
{
	int port2;///delete

	m_udp.unbind();
	m_listener.close();

	if(m_state == NWState::Server)
		if(m_listener.listen(port) != sf::Socket::Done)
		{
			cout << "\nListener There was an error binding to port [" << port << "]";
			///ERROR LOG
			messageLobbyLocal("There was a problem binding to the port.");
		}
	if(m_udp.bind(port + 1) != sf::Socket::Done)
	{
		cout << "\nUDP There was an error binding to port [" << port << "]";
		///ERROR LOG
		messageLobbyLocal("There was a problem binding to the port.");
		cin >> port2;
		return false;
	}
	return true;
}
void NetworkBoss::messageLobbyLocal(const std::string& rMessage)
{
	sf::Packet data;
	data << rMessage;
	Message mes("lobby_chatbox", "addLineLocal", data, 0, true);
	game.getCoreIO().recieve(mes);
}
void NetworkBoss::messageLobby(const std::string& rMessage)
{
	sf::Packet data;
	data << rMessage;
	Message mes("lobby_chatbox", "addLine", data, 0, true);
	game.getCoreIO().recieve(mes);
}
bool NetworkBoss::isClient() const
{
	return (m_state == NWState::Client);
}
NWState NetworkBoss::getNWState() const
{
	return m_state;
}
Connection* NetworkBoss::findConnection(const sf::IpAddress& rAdd, unsigned short fromUDPPort)/**FIND A CONNECTION IF WE HAVE ONE**/
{
	for(auto it = m_connections.begin(); it != m_connections.end(); ++it)
		if((*it)->getTcpSocket().getRemoteAddress() == rAdd && ((*it)->getTcpSocket().getRemotePort() + 1) == fromUDPPort)
			return it->get();

	return NULL;
}
bool NetworkBoss::hasConnections()/**DO WE HAVE CONNECTIONS?**/
{
	return (m_connections.size() > 0);
}
void NetworkBoss::addConnection(sptr<sf::TcpSocket> spTcpSocket, bool valid)/**SERVER ADDING CONNECTION**/
{
	m_connections.push_back(sptr<Connection>(new Connection(&m_udp, spTcpSocket, valid)));

	if(m_connections.back()->validated())
		m_connections.back()->sendTcp(Protocol::Handshake, voidPacket);
}
/**UTILITY**/






/**====STATE====**/
void NetworkBoss::setClient(const std::string& address, unsigned short port, float timeout)/**JOIN HOST**/
{

	setState(NWState::Client, false, false, false, true);

	m_timeOut = timeout;


	messageLobbyLocal("Connecting to [" + address + "]");  //message connecting

	sptr<sf::TcpSocket> spSocket(new sf::TcpSocket());
	spSocket->setBlocking(false);

	spSocket->connect(sf::IpAddress(address), port);
	setRecievePort(spSocket->getLocalPort());

	addConnection(spSocket, false);
	m_listener.close();
}
void NetworkBoss::setLocal()//we decide to be antisocial
{
	setState(NWState::Local, false, true, true, false);
	m_listener.close();
}
void NetworkBoss::setServer(unsigned short port, float timeout)//we decide to try and host
{
	setState(NWState::Server, true, true, false, true);

	m_timeOut = timeout;
	setRecievePort(port);

	std::ostringstream oss;
	oss << port;
	messageLobbyLocal("Hosting on port[" + oss.str() + "]");  //message "Hosting on port"
}
void NetworkBoss::setState(NWState state, bool open, bool acceptsLocal, bool hideLobby, bool hideConnectScreen)
{
	m_nwGameStarted = false;
	m_state = state;
	m_isOpen = open;
	m_connections.clear();
	game.getUniverse().getUniverseIO().toggleAcceptsLocal(acceptsLocal);

	Message clearChat("lobby_chatbox", "clear", voidPacket, 0, true);    //clear chat
	game.getCoreIO().recieve(clearChat);

	sf::Packet pack1;
	pack1 << hideLobby;
	Message hideLobbyMes("lobby", "setHidden", pack1, 0, true);          //show lobby
	game.getCoreIO().recieve(hideLobbyMes);

	sf::Packet pack2;
	pack2 << hideConnectScreen;
	Message hideMult("multiplayer_connect", "setHidden", pack2, 0, true); //hide multiplayer panel
	game.getCoreIO().recieve(hideMult);
}
/**====STATE====**/






void NetworkBoss::update()
{

	tcpListen();
	updateConnections();

	udpRecieve();
	tcpRecieve();

	sendUdp();
	sendTcp();

}

void NetworkBoss::udpRecieve()
{
	if(m_nwGameStarted)
	{
		sf::Packet data;
		sf::IpAddress fromIP;
		unsigned short fromPort;

		bool done = false;
		while(!done)
		{
			data.clear();
			if(m_udp.receive(data, fromIP, fromPort) == sf::Socket::Done)/**FOR EACH PACKET**/
			{
				Connection* pCon = findConnection(fromIP, fromPort);
				if(pCon != NULL)/**RECOGNIZED CONNECTION**/
				{
					Protocol proto = pCon->recievePacket(data);

					if(proto != Protocol::End)
					{
						if(proto == Protocol::Control)
							game.getUniverse().getControllerFactory().getNWFactory().process(data);
						else if(proto == Protocol::Data)
							m_nwFactory.process(data);
						else
							cout << "\n" << FILELINE << " [" << static_cast<int32_t>(proto) << "]";
					}
					else
					{
						cout << "\nProto End.";
					}
				}
			}
			else
				done = true;
		}
	}
}
void NetworkBoss::tcpRecieve()//receive data from each TcpPort (tcp)
{
	sf::Packet data;
	for(int32_t i = 0; i < (signed)m_connections.size(); ++i)
	{
		bool done = false;
		while(!done)
		{
			data.clear();
			if(m_connections[i]->getTcpSocket().receive(data) == sf::Socket::Done)/**FOR EACH PACKET**/
			{
				Protocol proto = m_connections[i]->recievePacket(data);

				if(proto != Protocol::End)
				{
					if(proto == Protocol::Tcp)
						m_nwFactoryTcp.process(data);
					else if(proto == Protocol::LoadLevel)
					{
						recieveLevel(data);
						cout << "\nLoad Level";
					}
					else if(proto == Protocol::Handshake)
					{
						m_connections.back()->setValid();
						messageLobbyLocal("Connection Successful");
					}
					else
						cout << "\n" << FILELINE << " [" << static_cast<int32_t>(proto) << "]";
				}
				else
				{
					cout << "\n" << FILELINE;
				}
			}
			else
				done = true;
		}
	}
}
void NetworkBoss::sendUdp()
{
	sf::Packet udpPacket;
	m_nwFactory.getData(udpPacket);//WE NEED TO SEND OUR NW game DATA
	for(int32_t i = 0; i < (signed)m_connections.size(); ++i)
		m_connections[i]->sendUdp(Protocol::Data, udpPacket);

	udpPacket.clear();
	game.getUniverse().getControllerFactory().getNWFactory().getData(udpPacket);//WE NEED TO SEND OUR CONTROLLER DATA
	for(int32_t i = 0; i < (signed)m_connections.size(); ++i)
		m_connections[i]->sendUdp(Protocol::Control, udpPacket);
}
void NetworkBoss::sendTcp()
{
	sf::Packet tcpPacket;
	m_nwFactoryTcp.getData(tcpPacket);//WE NEED TO SEND OUR NW game DATA
	for(int32_t i = 0; i < (signed)m_connections.size(); ++i)
		m_connections[i]->sendTcp(Protocol::Tcp, tcpPacket);
}
void NetworkBoss::tcpListen()//check for new connections
{
	if(m_state == NWState::Server)
	{
		sptr<sf::TcpSocket> spSocket(new sf::TcpSocket());
		spSocket->setBlocking(false);
		sf::Socket::Status fal = m_listener.accept(*spSocket);
		if(fal == sf::Socket::Done && m_isOpen)
		{
			int32_t players = m_connections.size() + 1;
			std::stringstream sst;
			sst << players;
			std::string num = sst.str();
			messageLobby("New Player Connected.");
			messageLobby(num + "players total.");
			std::cout << "\nNew connection received from [" << spSocket->getRemoteAddress() << "].";
			addConnection(spSocket, true);
		}
	}
}
void NetworkBoss::updateConnections()
{
	for(int32_t i = 0; i < (signed)m_connections.size(); ++i)
	{
		if(m_connections[i]->getStatus() == sf::Socket::Status::Disconnected && m_connections[i]->validated())//check if this connection is still working
		{
			if(m_state == NWState::Client)
			{
				setState(NWState::Local, false, true, true, false);//if we were a client, set us to local control
				cout << "\nDisconnected.";
			}
			else
			{
				m_connections.erase(m_connections.begin() + i);
				--i;
				cout << "\nConnection Dropped From Timeout";
				messageLobby("Player Disconnected");
			}
		}
	}
}






/// <summary>
/// Loads the level.
/// </summary>
/// <param name="data">The data.</param>
void NetworkBoss::recieveLevel(sf::Packet& data)//we are anyone being told to load the game
{
	m_nwGameStarted = true;
	std::string level;
	std::string blueprints;
	int32_t numControllers;
	std::string slave;
	std::string title;
	std::vector<std::string> controllerList;
	std::vector<std::string> shipTitleList;
	int32_t localController;


	data >> level;
	data >> blueprints;
	data >> numControllers;
	for(int32_t i = 0; i < numControllers; ++i)
	{
		data >> slave;
		data >> title;
		cout << "\n[" << slave << "][" << title << "]";
		controllerList.push_back(slave);
		shipTitleList.push_back(title);
	}
	data >> localController;
	cout << "\nCont" << localController;

	game.launchGame(level, localController, blueprints, controllerList, shipTitleList);
}
void NetworkBoss::launchMultiplayerGame()
{
	sf::Packet data;

	std::string level = "levels/level_1/";
	std::string blueprints = "blueprints/";

	data << level;
	data << blueprints;
	data << static_cast<int32_t>(m_connections.size() + 1);//number of controllers

	//host
	data << "1";
	data << "CombatShip";
	//for clients
	for(int32_t i = 0; i < (signed)m_connections.size(); ++i)
	{
		string slaveName = std::to_string(i + 1 + 1);
		string shipName = m_connections[i]->getShipChoice();
		cout << "\nSlave:[" << slaveName << "] title:[" << shipName << "].";
		data << slaveName;
		data << shipName;
	}

	int32_t controller = 0;
	sf::Packet hostData(data);
	hostData << controller++;

	for(auto it = m_connections.begin(); it != m_connections.end(); ++it)
	{
		sf::Packet launchData(data);
		launchData << controller++;
		(*it)->sendTcp(Protocol::LoadLevel, launchData);
	}

	recieveLevel(hostData);
}
/**REDUCTION**/


void NetworkBoss::input(const std::string rCommand, sf::Packet rData)
{
	if(rCommand == "joinIP")
	{
		rData >> m_joinIP;
	}
	else if(rCommand == "joinPort")
	{
		std::string port;
		rData >> port;
		std::stringstream sstream(port);
		sstream >> m_joinPort;
	}
	else if(rCommand == "joinTime")
	{
		std::string time;
		rData >> time;
		std::stringstream sstream(time);
		sstream >> m_timeOut;
	}
	else if(rCommand == "join")
	{
		if(m_joinIP != "" && m_joinPort > 1024)
			setClient(m_joinIP, m_joinPort, m_timeOut);
	}
	else if(rCommand == "host")
	{
		if(m_joinPort > 1024)
			setServer(m_joinPort, m_timeOut);
	}
	else if(rCommand == "localOnly")
	{
		setLocal();
	}
	else if(rCommand == "launch")
	{
		if(m_state != NWState::Client)
			launchMultiplayerGame();
	}
	else
	{
		cout << "\n" << FILELINE;
	}
}
