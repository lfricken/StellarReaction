#include "Connection.hpp"
#include "Globals.hpp"
#include "Universe.hpp"
#include "Player.hpp"
#include "Game.hpp"

Connection::Connection(sf::UdpSocket* pSocket, sptr<sf::TcpSocket> spTCPSocket, bool valid) : BasePlayerTraits("default")
{
	m_valid = valid;
	m_pUdpSocket = pSocket;
	m_spTcpSocket = spTCPSocket;
	m_status = sf::Socket::Status::Done;//TODO, Determine what m_status should be initialized to, and whether it is even used

	for(int32_t i = 0; i < static_cast<int32_t>(Protocol::End); ++i)
	{
		m_lastSendRecieve[static_cast<Protocol>(i)].first = 0;
		m_lastSendRecieve[static_cast<Protocol>(i)].second = 0;
	}
}
Connection::~Connection()
{
	Print << "\nConnection Destroyed!";
	m_spTcpSocket->disconnect();
}
void Connection::sendUdp(Protocol proto, const sf::Packet& rData)
{
	sf::Packet data;
	this->prepSend(proto, rData, data);
	m_pUdpSocket->send(data, m_spTcpSocket->getRemoteAddress(), m_spTcpSocket->getRemotePort() + 1);
}
void Connection::sendTcp(Protocol proto, const sf::Packet& rData)
{
	sf::Packet data;
	this->prepSend(proto, rData, data);
	m_status = m_spTcpSocket->send(data);
}
void Connection::prepSend(Protocol proto, const sf::Packet& rData, sf::Packet& data)
{
	data << static_cast<int32_t>(proto);
	data << ++m_lastSendRecieve[proto].first;
	data.append(rData.getData(), rData.getDataSize());
}
Protocol Connection::recievePacket(sf::Packet& rData)
{
	Protocol proto;

	int32_t proto32;
	int32_t sendID;
	rData >> proto32;
	rData >> sendID;
	proto = static_cast<Protocol>(proto32);

	if(sendID <= m_lastSendRecieve[proto].second)//if this sendID is old, we should disregard it
		return Protocol::End;

	m_lastSendRecieve[proto].second = sendID;
	return proto;
}
sf::TcpSocket& Connection::getTcpSocket()
{
	return *m_spTcpSocket;
}
sf::Socket::Status Connection::getStatus() const
{
	return m_status;
}
bool Connection::validated() const
{
	return m_valid;
}
void Connection::setValid()
{
	m_valid = true;
}
void Connection::syncPlayerTraits()
{
	sf::Packet message;


	sendUdp(Protocol::PlayerTraits, message);
}
void Connection::recievePlayerTraits(sf::Packet mes)
{

}
/// <summary>
/// this is a client connection on the server
/// the server has determined that a module should be added to this clients list of useable modules
/// the server will now send a packet to the client
/// </summary>
/// <param name="newTitle">The new title.</param>
/// <param name="rPos">The r position.</param>
//void Connection::addModule(const String& newTitle, const sf::Vector2i& rPos)
//{
//	//m_owned.push_back(pair<String, Vec2>(newTitle, rPos));
//	sf::Packet pack;
//	pack << newTitle;
//	pack << rPos.x;
//	pack << rPos.y;
//	Message modAdded("ship_editor", "addModule ToGui", pack, 0.f, false);
//	this->sendSpecialIo(modAdded);
//}
void Connection::sendSpecialIo(const Message& mes)
{
	sf::Packet packet;


	/**TARGETS**/
	packet << (int32_t)mes.getTargetPosition();
	packet << mes.getTargetName();
	/**COMMAND**/
	packet << mes.getCommand();
	/**DATA**/
	int dataSize = (signed)mes.getData().getDataSize();
	packet << (int32_t)dataSize;
	int8_t* pByte = (int8_t*)mes.getData().getData();
	for(int chard = 0; chard < dataSize; ++chard)
	{
		int8_t byte = pByte[chard];
		packet << byte;
	}
	/**DELAY**/
	packet << mes.getDelay();


	this->sendTcp(Protocol::SpecialIoEvent, packet);
}
void Connection::recieveSpecialIo(sf::Packet& rPacket)
{
		int32_t pos;
		String name;
		String command;
		int32_t size;
		int8_t* pData;
		sf::Packet messageData;
		float delay;

		/**TARGETS**/
		rPacket >> pos;
		rPacket >> name;
		/**COMMAND**/
		rPacket >> command;

		/**DATA**/
		rPacket >> size;
		pData = new int8_t[size];
		for(int chard = 0; chard < size; ++chard)
		{
			int8_t byte;
			rPacket >> byte;
			pData[chard] = byte;
		}
		messageData.append(pData, size);
		/**DELAY**/
		rPacket >> delay;

		Message mes((unsigned int)pos, command, messageData, delay, false);
		mes.setName(name);
		getGame()->getCoreIO().recieve(mes);
		
		
		delete[] pData;
}
