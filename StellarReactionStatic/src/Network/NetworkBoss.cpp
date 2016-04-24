#include "NetworkBoss.hpp"
#include "Protocol.hpp"
#include "IOManager.hpp"
#include "Universe.hpp"
#include "Player.hpp"
#include "SlaveLocator.hpp"
#include "Chunk.hpp"
#include "BlueprintLoader.hpp"
#include "Module.hpp"

using namespace std;
using namespace sf;

/// <summary>
/// anyone being told to load the game
/// </summary>
void NetworkBoss::recieveLevel(sf::Packet& data)
{
	m_nwGameStarted = true;
	GameLaunchData launchData;

	/**Level**/
	data >> launchData.level;

	/**List of Player Data**/
	int32_t numControllers;
	data >> numControllers;
	for(int32_t i = 0; i < numControllers; ++i)
	{
		GameLaunchData::PlayerData playerInstance;

		data >> playerInstance.slaveName;
		data >> playerInstance.playerName;
		data >> playerInstance.playerMoney;
		data >> playerInstance.ship;
		data >> playerInstance.team;
		data >> playerInstance.isAI;
		//cout << "\nControllers: " << numControllers;
		//cout << "\nSlaveName:[" << playerInstance.slaveName << "] Ship Choice:[" << playerInstance.ship << "] Team:[" << playerInstance.team << "]";
		launchData.playerList.push_back(playerInstance);
	}
	/**Local Controller**/
	data >> launchData.localController;
	///cout << "\nLocal Controller: [" << launchData.localController << "]";

	/**Launch the game!**/
	game.launchGame(launchData);
}
/// <summary>
/// server deciding to launch the game
/// </summary>
void NetworkBoss::launchMultiplayerGame()
{
	sf::Packet data;

	std::string level = "Alpha Centauri";

	data << level;
	data << static_cast<int32_t>(m_connections.size() + 1);//number of controllers +1 for host

	//host
	data << "1";
	data << game.getLocalPlayer().getName();
	data << game.getLocalPlayer().getMoney();
	data << game.getLocalPlayer().getShipChoice();
	data << game.getLocalPlayer().getTeam();
	data << false;

	//for clients
	for(int32_t i = 0; i < (signed)m_connections.size(); ++i)
	{
		string slaveName = std::to_string(i + 1 + 1);//+1 for host, +1 for index offset
		sptr<Connection> client = m_connections[i];
		string playerName = client->getName();
		Money playerMoney = client->getMoney();
		string shipName = client->getShipChoice();
		int team = client->getTeam();
		cout << "\nSlave:[" << slaveName << "] title:[" << shipName << "].";
		data << slaveName;
		data << playerName;
		data << playerMoney;
		data << shipName;
		data << team;
		data << false;//is ai
	}


	/*
	for (int i = 1; i <= 4; ++i)
	{
		string aiSlaveName = std::to_string(m_connections.size() + 20 + i);
		string aiShipName = "Dante";
		int aiTeam = i;
		data << aiSlaveName;
		data << "AI_PLAYER";
		data << (int)0;
		data << aiShipName;
		data << aiTeam;
		data << true;
	}
	*/

	int32_t controller = 0;
	sf::Packet hostData(data);
	hostData << controller;
	++controller;

	for(auto it = m_connections.begin(); it != m_connections.end(); ++it)
	{
		sf::Packet launchData(data);
		(*it)->setController(controller);
		launchData << controller;
		++controller;
		(*it)->sendTcp(Protocol::LoadLevel, launchData);
	}

	recieveLevel(hostData);
}
NetworkBoss::NetworkBoss(const NetworkBossData& rData) : m_io(rData.ioComp, &NetworkBoss::input, this), m_nwFactory("standard"), m_nwFactoryTcp("tcp")
{
	m_state = NWState::Local;
	m_udp.unbind();
	m_udp.setBlocking(false);

	m_remoteIP = "127.0.0.1";
	m_port = 5050;

	m_timeOut = 10.f;//how long the timeout is
	m_nwGameStarted = false;
	m_isOpen = false;//are we accepting connections

	m_listener.setBlocking(false);
}
NetworkBoss::~NetworkBoss()
{
	m_udp.unbind();
}
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
/// <summary>
/// anyone, which ports should we check?
/// </summary>
bool NetworkBoss::setRecievePort(unsigned short localPort)
{
	m_udp.unbind();
	m_listener.close();
	bool worked = true;

	if(m_state == NWState::Server)
		if(m_listener.listen(localPort) != sf::Socket::Done)
			worked = false;

	if(m_udp.bind(localPort + 1) != sf::Socket::Done)
		worked = false;

	if(worked)
		return true;
	else
	{
		cout << FILELINE;
		m_udp.unbind();
		m_listener.close();
		return false;
	}
}
void NetworkBoss::messagePlayerCount()
{
	if(getNWState() == NWState::Server)
		messageLobby(std::to_string(m_connections.size() + 1) + " players total.");
}
/// <summary>
/// anyone send a message to their local lobby (only our computer)
/// </summary>
void NetworkBoss::messageLobbyLocal(const std::string& rMessage)
{
	sf::Packet data;
	data << rMessage;
	Message mes("lobby_chatbox", "addLineLocal", data, 0, true);
	game.getCoreIO().recieve(mes);
}
/// <summary>
/// anyone send a message to everyone
/// </summary>
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
std::vector<sptr<Connection> >& NetworkBoss::getConnections()
{
	return m_connections;
}
Connection* NetworkBoss::findConnection(const sf::IpAddress& remoteAddress, unsigned short remotePort)/**FIND A CONNECTION IF WE HAVE ONE**/
{
	for(auto it = m_connections.begin(); it != m_connections.end(); ++it)
		if((*it)->getTcpSocket().getRemoteAddress() == remoteAddress && ((*it)->getTcpSocket().getRemotePort() + 1) == remotePort)
			return it->get();

	return NULL;
}
/// <summary>
/// anyone Do we have connections?
/// </summary>
bool NetworkBoss::hasConnections()
{
	return (m_connections.size() > 0);
}
/// <summary>
/// Adds the connection.
/// </summary>
void NetworkBoss::addConnection(sptr<sf::TcpSocket> spTcpSocket, bool valid)/**SERVER ADDING CONNECTION**/
{
	m_connections.push_back(sptr<Connection>(new Connection(&m_udp, spTcpSocket, valid)));

	if(m_connections.back()->validated())
		m_connections.back()->sendTcp(Protocol::Handshake, voidPacket);
}
/// <summary>
/// anyone deciding to try and connect
/// </summary>
void NetworkBoss::setClient(const std::string& remoteAddress, unsigned short remotePort, float timeout)/**JOIN HOST**/
{

	setState(NWState::Client, false, false, false, true);

	m_timeOut = timeout;
	std::ostringstream oss;
	oss << remotePort;
	messageLobbyLocal("Connecting to [" + remoteAddress + ":" + oss.str() + "].");  //message connecting

	sptr<sf::TcpSocket> spSocket(new sf::TcpSocket());
	spSocket->setBlocking(false);

	spSocket->connect(sf::IpAddress(remoteAddress), remotePort);
	unsigned short localPort = spSocket->getLocalPort();
	setRecievePort(localPort);

	addConnection(spSocket, false);
	m_listener.close();
}
/// <summary>
/// Anyone decide to be antisocial
/// </summary>
void NetworkBoss::setLocal()
{
	setState(NWState::Local, false, true, true, false);
	m_listener.close();
}
/// <summary>
/// anyone decides to host on a port
/// </summary>
void NetworkBoss::setServer(unsigned short port, float timeout)//we decide to try and host
{
	setState(NWState::Server, true, true, false, true);

	m_timeOut = timeout;
	bool hostWorked = setRecievePort(port);


	std::ostringstream oss;
	oss << port;
	if(hostWorked)
		messageLobbyLocal("Hosting on port [" + oss.str() + "]");  //message "Hosting on port"
	else
		messageLobbyLocal("Failed to bind to port [" + oss.str() + "]. Try hosting on a different port.");  //message "Hosting on port"

}
/// <summary>
/// Sets the state.
/// </summary>
/// <param name="state">Our State</param>
/// <param name="open">Are we open to new connections.</param>
/// <param name="acceptsLocal">Universal IO accepts Local messages?</param>
/// <param name="hideLobby">Should we hide the lobby screen?</param>
/// <param name="hideConnectScreen">Should we hide the connect screen?</param>
void NetworkBoss::setState(NWState state, bool open, bool acceptsLocal, bool hideLobby, bool hideConnectScreen)
{
	m_nwGameStarted = false;
	m_state = state;
	m_isOpen = open;
	m_connections.clear();
	game.getUniverse().getUniverseIO().toggleAcceptsLocal(acceptsLocal);

	Message clearChat("lobby_chatbox", "clear", voidPacket, 0, true);//clear chat
	game.getCoreIO().recieve(clearChat);

	sf::Packet pack1;
	pack1 << hideLobby;
	Message hideLobbyMes("lobby", "setHidden", pack1, 0, true);//show lobby
	game.getCoreIO().recieve(hideLobbyMes);

	sf::Packet pack2;
	pack2 << hideConnectScreen;
	Message hideMult("multiplayer_connect", "setHidden", pack2, 0, true); //hide multiplayer panel
	game.getCoreIO().recieve(hideMult);
}
/// <summary>
/// Listen for data, and send data.
/// </summary>
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
		sf::IpAddress remoteIP;
		unsigned short remotePort;

		bool done = false;
		while(!done)
		{
			data.clear();
			if(m_udp.receive(data, remoteIP, remotePort) == sf::Socket::Done)/**FOR EACH PACKET**/
			{
				Connection* pCon = findConnection(remoteIP, remotePort);
				if(pCon != NULL)//the message is from one of our connections
				{
					Protocol proto = pCon->recievePacket(data);

					if(proto != Protocol::End)
					{
						if(proto == Protocol::Control)
							game.getUniverse().getControllerFactory().getNWFactory().process(data);
						else if(proto == Protocol::Data)
							m_nwFactory.process(data);
						else if(proto == Protocol::PlayerTraits)
							pCon->recievePlayerTraits(data);
						else
							cout << "\n" << FILELINE << " [" << static_cast<int32_t>(proto) << "]";
					}
					else
					{
						//cout << FILELINE;
					}
				}
				else//if we don't regonize the remoteAddress, we should ignore it
					cout << FILELINE;
			}
			else
				done = true;
		}
	}
}
/// <summary>
/// anyone receive data from each TcpPort (tcp)
/// </summary>
void NetworkBoss::tcpRecieve()
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
					}
					else if(proto == Protocol::PlayerOption)//server recieving something
					{
						//Server recieving
						if(getNWState() == NWState::Server)
							playerOption(data, m_connections[i].get());
						else
							cout << FILELINE;
					}
					else if(proto == Protocol::ReturnHandshake)//server recieving the return handshake
					{
						if(getNWState() == NWState::Server)
						{
							messageLobby(m_connections[i]->getName() + " has joined the game!");
							messagePlayerCount();
						}
						else
							cout << FILELINE;
					}
					else if(proto == Protocol::Handshake)//client being told we succesfully connected
					{
						m_connections.back()->setValid();
						messageLobbyLocal("Connection Successful");

						/**SET OUR DATA**/
						sf::Packet setNamePacket;

						setNamePacket << "setName";
						setNamePacket << game.getLocalPlayer().getName();

						if(m_connections.back()->validated())
						{
							m_connections.back()->sendTcp(Protocol::PlayerOption, setNamePacket);
							m_connections.back()->sendTcp(Protocol::ReturnHandshake, voidPacket);
						}
						/**SET OUR DATA**/
					}
					else if(proto == Protocol::SpecialIoEvent)
					{
						m_connections.back()->recieveSpecialIo(data);
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
/// <summary>
/// Anyone sending via UDP to all connections
/// </summary>
void NetworkBoss::sendUdp()
{
	sf::Packet udpPacket;
	static int counter = 0;
	const int frequency = 6;
	if(counter >= frequency)
	{
		counter = 0;
		//universe component data
		if(getNWState() == NWState::Server)
		{
			m_nwFactory.getComponentData(udpPacket);
			for(int32_t i = 0; i < (signed)m_connections.size(); ++i)
				m_connections[i]->sendUdp(Protocol::Data, udpPacket);
		}
	}

	//controller data
	udpPacket.clear();
	game.getUniverse().getControllerFactory().getNWFactory().getComponentData(udpPacket);
	for(int32_t i = 0; i < (signed)m_connections.size(); ++i)
		m_connections[i]->sendUdp(Protocol::Control, udpPacket);
	++counter;
}
/// <summary>
/// Anyone sending via TCP to all connections
/// </summary>
void NetworkBoss::sendTcp()
{
	//special unloseable data
	sf::Packet tcpPacket;
	m_nwFactoryTcp.getComponentData(tcpPacket);//WE NEED TO SEND OUR NW game DATA
	for(int32_t i = 0; i < (signed)m_connections.size(); ++i)
		m_connections[i]->sendTcp(Protocol::Tcp, tcpPacket);
}
/// <summary>
/// server check for new connections
/// </summary>
void NetworkBoss::tcpListen()
{
	if(m_state == NWState::Server)
	{
		sptr<sf::TcpSocket> spSocket(new sf::TcpSocket());
		spSocket->setBlocking(false);
		sf::Socket::Status fal = m_listener.accept(*spSocket);
		if(fal == sf::Socket::Done && m_isOpen)
			addConnection(spSocket, true);
	}
}
/// <summary>
/// anyone Check connections to see status, and see if we should drop anyone.
/// </summary>
void NetworkBoss::updateConnections()
{
	static int counter = 0;
	const int frequency = 10;
	for(int32_t i = 0; i < (signed)m_connections.size(); ++i)
	{
		if(m_connections[i]->getStatus() == sf::Socket::Status::Disconnected && m_connections[i]->validated())//check if this connection is still working
		{
			if(m_state == NWState::Client)
			{
				setState(NWState::Local, false, true, true, false);//if we were a client, set us to local control
				messageLobbyLocal("Disconnected from server.");
			}
			else
			{
				messageLobby(m_connections[i]->getName() + " Disconnected.");
				m_connections.erase(m_connections.begin() + i);
				messagePlayerCount();
				--i;
			}
		}
		//server only
		else if(counter >= frequency && getNWState() == NWState::Server && m_connections[i]->validated())
		{
			m_connections[i]->syncPlayerTraits();
		}
	}
	if(counter >= frequency)
		counter = 0;
	++counter;

}
/// <summary>
/// server being given player information
/// pFrom is the target player. This could be us, in which case we are a BasePlayerTrait,
/// or it could be a Connection, representing a client
/// </summary>
void NetworkBoss::playerOption(sf::Packet& rData, BasePlayerTraits* pFrom)
{
	string command;
	rData >> command;
	if(command == "setShip")
	{
		string shipName;
		rData >> shipName;
		pFrom->setShipChoice(shipName);
		messageLobby(pFrom->getName() + " changed ship to [" + shipName + "].");
	}
	else if(command == "setTeam")
	{
		string steam;
		rData >> steam;
		int team = stoi(steam);
		pFrom->setTeam(team);
		messageLobby(pFrom->getName() + " changed to team [" + to_string(team) + "].");
	}
	else if(command == "setName")
	{
		string name;
		rData >> name;
		pFrom->setName(name);
	}
	else if(command == "buyModule")
	{
		string bpName;
		rData >> bpName;

		const ModuleData* pMod = game.getUniverse().getBlueprints().getModuleSPtr(bpName).get();
		if(pMod != NULL)
		{
			Money cost = pMod->cost;//see if we can afford it
			if(pFrom->getMoney() >= cost)
			{
				pFrom->addModule(bpName, b2Vec2(0, 0));//tell whoever this command came from to add a module to their gui
				pFrom->changeMoney(-cost);
			}
		}
		else
			cout << FILELINE;
	}
	else if(command == "addModule")//explicitly adds a module to the gui of pFrom, used to set up initial ship
	{
		string bpName;
		float x = 0;
		float y = 0;
		rData >> bpName;
		rData >> x;
		rData >> y;
		const ModuleData* pMod = game.getUniverse().getBlueprints().getModuleSPtr(bpName).get();
		if(pMod != NULL)
		{
			pFrom->addModule(bpName, b2Vec2(x, y));//tell whoever this command came from to add a module to their gui
			//just assume they already paid for it, or aquired it otherwise
		}
		else
			cout << FILELINE;
	}
	else if(command == "rebuild")//a player wants to attach a module to thier ship, from their inventory
	{
		string bpName;
		float x;
		float y;
		vector<pair<string, Vector2f> > modules;

		Controller* pCon = &game.getUniverse().getControllerFactory().getController(pFrom->getController());
		std::string slaveName = pCon->getSlaveName();
		Chunk* pTemp = game.getUniverse().getSlaveLocator().findHack(slaveName);
		int targetPos = pTemp->m_io.getPosition();

		Message clean((unsigned)targetPos, "clear", voidPacket, 0, false);
		clean.sendOverNW(true);
		game.getUniverse().getUniverseIO().recieve(clean);

		vector<pair<string, b2Vec2> > available = pFrom->getOwnedModuleTitles();

		int num;
		rData >> num;
		for(int i = 0; i < num; ++i)
		{
			rData >> bpName;
			rData >> x;
			rData >> y;

			bool canAdd = false;
			//can we add this module?
			for(auto it = available.begin(); it != available.end(); ++it)
			{
				if(it->first == bpName)
				{
					canAdd = true;
					available.erase(it);
					break;
				}
			}

			if(canAdd)//if the user had that in their inventory
			{//then we just removed it, and we should add it to thier ship
				sf::Packet pack;
				pack << bpName;
				pack << x;
				pack << y;

				Message attach((unsigned)targetPos, "attachModule", pack, 0, false);
				attach.sendOverNW(true);
				game.getUniverse().getUniverseIO().recieve(attach);
			}
		}
	}
	else
		cout << FILELINE << " [" << command << "].";
}
//Local
void NetworkBoss::input(const std::string rCommand, sf::Packet rData)
{
	if(rCommand == "joinIP")
	{
		rData >> m_remoteIP;
	}
	else if(rCommand == "joinPort")
	{
		std::string port;
		rData >> port;
		std::stringstream sstream(port);
		sstream >> m_port;
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
		if(m_remoteIP != "" && m_port > 1024)
			setClient(m_remoteIP, m_port, m_timeOut);
	}
	else if(rCommand == "host")
	{
		if(m_port > 1024)
			setServer(m_port, m_timeOut);
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
	///If we are the host, handle our own message.
	///If we are a client, send it to the server.
	else if(rCommand == "sendTcpToHost")
	{
		if(getNWState() == NWState::Server)
			playerOption(rData, &game.getLocalPlayer());
		else
			for(auto it = m_connections.begin(); it != m_connections.end(); ++it)
				(*it)->sendTcp(Protocol::PlayerOption, rData);
	}
	else
	{
		cout << "\n" << FILELINE;
	}
}
