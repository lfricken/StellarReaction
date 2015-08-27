#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "stdafx.hpp"
#include "Protocol.hpp"

class Connection
{
public:
	Connection(sf::UdpSocket* pSocket, sptr<sf::TcpSocket> spTcpSocket, bool valid);
	~Connection();

	void sendUdp(Protocol proto, const sf::Packet& rData);//send data via a udp packet
	void sendTcp(Protocol proto, const sf::Packet& rData);//send data via a tcp packet
	Protocol recievePacket(sf::Packet& rData);//what do we do with this packet?, if End, then the packet should be ignored

	sf::TcpSocket& getTcpSocket();
	sf::Socket::Status getStatus() const;
	bool validated() const;//did we get validated
	void setValid();
	void setShipChoice(const std::string& rTitle);
	const std::string& getShipChoice() const;
protected:
private:
	void prepSend(Protocol proto, const sf::Packet& rData, sf::Packet& data);

	sf::Socket::Status m_status;//last status returned to us
	bool m_valid;//has this connection been established?

	sf::UdpSocket* m_pUdpSocket;
	sptr<sf::TcpSocket> m_spTcpSocket;//each connection has a TCP port

	std::map<Protocol, std::pair<int32_t, int32_t> > m_lastSendRecieve;//for each type of protocol, what was the last send?
	std::string m_shipChoice;
};


#endif // CONNECTION_HPP
