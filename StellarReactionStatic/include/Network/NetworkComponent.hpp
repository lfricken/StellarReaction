#pragma once

#include "stdafx.hpp"
#include "NetworkBoss.hpp"
#include "NonCopyable.hpp"

/// \brief Data do initialize NetworkComponent
struct NetworkComponentData
{
	NetworkComponentData()
	{

	}

	/// Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};

/// \brief Syncs data between objects on different computers.
class NetworkComponent : NonCopyable
{
public:
	template <typename T>
	NetworkComponent(const NetworkComponentData& rData, void (T::*pack)(sf::Packet&), void (T::*unpack)(sf::Packet&), T* const classPtr, NetworkFactory& factory) : m_pFactory(&factory)
	{
		m_packFunction = std::bind(pack, classPtr, std::placeholders::_1);
		m_unpackFunction = std::bind(unpack, classPtr, std::placeholders::_1);
		m_newData = false;

		m_factoryID = m_pFactory->give(this);///WE NEED TO GIVE NW FACTORY THIS
	}
	virtual ~NetworkComponent();

	/// Put data into packet to be sent to clone on another computer.
	void pack(sf::Packet& rPacket);
	/// Extract data from packet, which was data sent from another computer.
	void unpack(sf::Packet& rPacket);

	/// Toggle flag whether we have new data to send.
	bool toggleNewData(bool newData);
	/// Return whether we have new data to send.
	bool hasNewData();

protected:
private:
	std::function<void(sf::Packet&)> m_packFunction;//the function we call when we get a receive call
	std::function<void(sf::Packet&)> m_unpackFunction;//the function we call when we get a receive call

	NetworkFactory* m_pFactory;
	int m_factoryID;
	bool m_newData;//is there new data to be sent?
};

