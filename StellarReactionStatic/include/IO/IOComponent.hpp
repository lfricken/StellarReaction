#pragma once

#include "EventTypes.hpp"
#include "stdafx.hpp"
#include "Courier.hpp"
#include "Eventer.hpp"
#include "IOManager.hpp"
#include "NonCopyable.hpp"


/// Used to initialize an IOComponent.
struct IOComponentData
{
	IOComponentData(IOManager* manager) :
		name("NONAME"),
		pMyManager(manager)
	{

	}
	std::string name;//the name we are located by
	std::vector<Courier> courierList;//list of couriers to event on
	IOManager* pMyManager;//pointer to our manager (to send a ptr to us to and for sending messages
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};

/// A class can be given this to allow it to send messages to other IOComponents on other objects.
class IOComponent : NonCopyable
{
public:

	/// Takes a function to call when an message has been sent to us, such as damage
	template <typename T>
	IOComponent(const IOComponentData& rData, void (T::*func)(std::string, sf::Packet), T* const classPtr) : m_pManager(rData.pMyManager), m_name(rData.name), m_eventer(*m_pManager)
	{
		m_cbFunction = std::bind(func, classPtr, std::placeholders::_1, std::placeholders::_2);
		m_ioManPosition = m_pManager->give(this);
		m_eventer.add(rData.courierList);
	}
	virtual ~IOComponent();
	/// Receive an event call.
	void event(EventType type, int value, const sf::Packet& rData);//EventType occurred, has value, and other data	
	/// Receive a command from a packet.
	void recieve(const std::string& rCommand, const sf::Packet& rData);
	/// Return the name of this IOComponent.
	const std::string& getName() const;
	/// Return the position of this IOComponent in the list.
	int getPosition() const;

protected:
private:
	IOManager* m_pManager;//a convienent reference to our manager
	std::string m_name;//our IO name
	int m_ioManPosition;//position of us in the io manager

	Eventer m_eventer;//the item that stores our conditions and messages
	std::function<void(std::string, sf::Packet)> m_cbFunction;//the function we call when we get a receive message
};

