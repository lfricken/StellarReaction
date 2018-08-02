#pragma once

#include "stdafx.hpp"
#include "NonCopyable.hpp"

class NetworkComponent;

/// \brief Holds list of NetworkComponent
///
/// Will sync data between games, by sending data pairwise between NetworkFactory lists.
class NetworkFactory : NonCopyable
{
public:
	NetworkFactory(String name);
	virtual ~NetworkFactory();

	/// A NetworkComponent calls this to give a pointer to itself.
	int give(NetworkComponent* pComponent);
	/// Resize list to minmize the list size, since some of the pointers may have been set to Null.
	void clean();
	/// Mark the pointer as null and offer it as a position to a future NetworkComponent
	void freeThis(int position);
	/// Get data from all the NetworkComponents to send to another getGame()->
	void getComponentData(sf::Packet& rPacket);
	/// Receive data from another Game, and process it.
	void process(sf::Packet& rPacket);

protected:
private:
	List<NetworkComponent*> m_componentPtrs;//where we store all of them
	int m_lastSendID;

	String m_name;
	/// <summary>
	/// How many times in a row have we gotten bad data.
	/// </summary>
	int m_consecutiveDesyncs;
};

