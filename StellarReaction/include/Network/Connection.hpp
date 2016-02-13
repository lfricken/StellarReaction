#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "stdafx.hpp"
#include "Protocol.hpp"
#include "BasePlayerTraits.hpp"
#include "Message.hpp"

/// <summary>
/// A connection to another player, 1 is used by client, 1 for each client used by server
/// </summary>
class Connection : public BasePlayerTraits
{
public:
	Connection(sf::UdpSocket* pSocket, sptr<sf::TcpSocket> spTcpSocket, bool valid);
	~Connection();

	void sendUdp(Protocol proto, const sf::Packet& rData);//send data via a udp packet
	void sendTcp(Protocol proto, const sf::Packet& rData);//send data via a tcp packet
	Protocol recievePacket(sf::Packet& rData);//this connection sent us a packet, return what we should do with it

	sf::TcpSocket& getTcpSocket();
	sf::Socket::Status getStatus() const;//status of tcp socket
	bool validated() const;//was this connection established?
	void setValid();//make our conection valid

	void syncPlayerTraits();//keeps clients updated to their money status (possibly other things)
	void recievePlayerTraits(sf::Packet mes);//keeps clients updated to their money status (possibly other things)

	void sendSpecialIo(const Message& mes);//called when an IOComponent message is sent across the network
	void recieveSpecialIo(sf::Packet& mes);//called when an IOComponent message is sent across the network

	/// <summary>
	/// this is a client connection on the server
	/// the server has determined that a module should be added to this clients list of useable modules
	/// the server will now send a packet to the client
	/// </summary>
	/// <param name="newTitle">The new title.</param>
	/// <param name="rPos">The r position.</param>
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
