#include "Connection.hpp"

using namespace std;

Connection::Connection(sf::UdpSocket* pSocket, sptr<sf::TcpSocket> spTCPSocket, bool valid)
{
	m_valid = valid;
	m_pUdpSocket = pSocket;
	m_spTcpSocket = spTCPSocket;

	for(int32_t i = 0; i<static_cast<int32_t>(Protocol::End); ++i)
	{
		m_lastSendRecieve[static_cast<Protocol>(i)].first = 0;
		m_lastSendRecieve[static_cast<Protocol>(i)].second = 0;
	}

	m_shipChoice = "CombatShip";
}
Connection::~Connection()
{
	cout << "\nConnection Destroyed!";
	m_spTcpSocket->disconnect();
}
void Connection::sendUdp(Protocol proto, const sf::Packet& rData)
{
	sf::Packet data;
	prepSend(proto, rData, data);
	m_pUdpSocket->send(data, m_spTcpSocket->getRemoteAddress(), m_spTcpSocket->getRemotePort()+1);
}
void Connection::sendTcp(Protocol proto, const sf::Packet& rData)
{
	sf::Packet data;
	prepSend(proto, rData, data);
	m_status = m_spTcpSocket->send(data);
}
void Connection::prepSend(Protocol proto, const sf::Packet& rData, sf::Packet& data)
{
	data << static_cast<int32_t>(proto);
	data << ++m_lastSendRecieve[proto].first;
	data.append(rData.getData(), rData.getDataSize());
}
Protocol Connection::recievePacket(sf::Packet& rData)//what do we do with this packet?, if End, then the packet should be ignored
{
	Protocol proto;

	int32_t proto32;
	int32_t sendID;
	rData >> proto32;
	rData >> sendID;
	proto = static_cast<Protocol>(proto32);

	if(sendID < m_lastSendRecieve[proto].second)//if this sendID is old...
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
void Connection::setShipChoice(const std::string& rTitle)
{
	m_shipChoice = rTitle;
}
const std::string&  Connection::getShipChoice() const
{
	return m_shipChoice;
}