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
	String name;//the name we are located by
	List<Courier> courierList;//list of couriers to event on
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
	IOComponent(const IOComponentData& rData, void (T::*func)(String, sf::Packet), T* const classPtr) : m_pManager(rData.pMyManager), m_name(rData.name), m_eventer(*m_pManager)
	{
		m_cbFunction = std::bind(func, classPtr, std::placeholders::_1, std::placeholders::_2);
		m_ioManPosition = m_pManager->give(this);
		m_eventer.add(rData.courierList);
	}
	virtual ~IOComponent();
	/// Receive an event call.
	void event(EventType type, int value, const sf::Packet& rData);//EventType occurred, has value, and other data	
	/// Receive a command from a packet.
	void recieve(const String& rCommand, const sf::Packet& rData);
	/// Return the name of this IOComponent.
	const String& getName() const;
	/// Return the position of this IOComponent in the list.
	int getPosition() const;

	Eventer& getEventer();

protected:
private:
	IOManager* m_pManager;//a convienent reference to our manager
	String m_name;//our IO name
	int m_ioManPosition;//position of us in the io manager
	/// <summary>
	/// The item that stores our conditions and messages. has to appear after m_pManager in this class.
	/// </summary>
	Eventer m_eventer;

	std::function<void(String, sf::Packet)> m_cbFunction;//the function we call when we get a receive message
};

