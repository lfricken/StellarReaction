#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "stdafx.hpp"
#include "Protocol.hpp"
#include "BasePlayerTraits.hpp"
#include "Message.hpp"
#include "NonCopyable.hpp"


/// A connection to another player.
/// 1 is used by the Client to stay connected to the server.
/// 1 for each client used by the Server.
class Connection : public BasePlayerTraits, NonCopyable
{
public:
	Connection(sf::UdpSocket* pSocket, sptr<sf::TcpSocket> spTcpSocket, bool valid);
	~Connection();

	/// Send data via a udp packet and a given Protocol.
	void sendUdp(Protocol proto, const sf::Packet& rData);
	/// Send data via a tcp packet and a given Protocol.
	void sendTcp(Protocol proto, const sf::Packet& rData);
	/// This connection sent us a packet, return what we should do with it.
	Protocol recievePacket(sf::Packet& rData);

	/// Return the SFML TcpSocket.
	sf::TcpSocket& getTcpSocket();
	/// Return the status of our tcp socket.
	sf::Socket::Status getStatus() const;
	/// Was this connection established?
	bool validated() const;
	/// Set this connection to be established.
	void setValid();

	/// As a Server, send player info, such as Money.
	void syncPlayerTraits();
	/// As a Client, recieve my player info, such as Money.
	void recievePlayerTraits(sf::Packet mes);

	/// Called when an IOComponent message is sent across the network.
	void sendSpecialIo(const Message& mes);
	void recieveSpecialIo(sf::Packet& mes);

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
