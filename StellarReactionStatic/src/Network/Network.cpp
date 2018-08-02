#include "Network.hpp"
#include "stdafx.hpp"
#include "Globals.hpp"
#include "Message.hpp"
#include "IOManager.hpp"

Network::Network()
{

}
Network::~Network()
{

}
void Network::toHostAtomic(sf::Packet data)
{
	Message mes("networkboss", "Protocol::PlayerOption", data, 0, false);
	getGame()->getCoreIO().recieve(mes);
}
