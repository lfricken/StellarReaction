#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "stdafx.hpp"
#include "Protocol.hpp"
#include "BasePlayerTraits.hpp"
#include "Message.hpp"

class Connection : public BasePlayerTraits
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

	void syncPlayerTraits();
	void recievePlayerTraits(sf::Packet mes);

	void sendSpecialIo(const Message& mes);
	void recieveSpecialIo(sf::Packet& mes);

	virtual void addModule(const std::string& newTitle, const b2Vec2& rPos);

protected:
private:
	void prepSend(Protocol proto, const sf::Packet& rData, sf::Packet& data);

	sf::Socket::Status m_status;//last status returned to us
	bool m_valid;//has this connection been established?

	sf::UdpSocket* m_pUdpSocket;
	sptr<sf::TcpSocket> m_spTcpSocket;//each connection has a TCP port

	std::map<Protocol, std::pair<int32_t, int32_t> > m_lastSendRecieve;//for each type of protocol, what was the last sendID, and what was the lastRecieveID?
};

#endif // CONNECTION_HPP
