#ifndef IOCOMPONENT_HPP
#define IOCOMPONENT_HPP

#include "EventTypes.hpp"
#include "stdafx.hpp"
#include "Courier.hpp"
#include "Eventer.hpp"
#include "IOManager.hpp"
#include "NonCopyable.hpp"


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

	virtual void loadJson(const Json::Value& root);
};

/// <summary>
/// A class can be given this to allow it to send messages to other IOComponents on other objects
/// </summary>
class IOComponent : NonCopyable
{
public:

	
	/// <summary>
	/// Takes a function to call when an message has been sent to us, such as damage
	/// </summary>
	/// <param name="rData">The r data.</param>
	/// <param name="func">The function.</param>
	/// <param name="classPtr">The class PTR.</param>
	template <typename T>
	IOComponent(const IOComponentData& rData, void (T::*func)(std::string, sf::Packet), T* const classPtr) : m_pManager(rData.pMyManager), m_name(rData.name), m_eventer(*m_pManager)
	{
		m_cbFunction = std::bind(func, classPtr, std::placeholders::_1, std::placeholders::_2);
		m_ioManPosition = m_pManager->give(this);
		m_eventer.add(rData.courierList);
	}
	virtual ~IOComponent();
	/// <summary>
	/// Called when an event happens.
	/// </summary>
	void event(EventType type, int value, const sf::Packet& rData);//EventType occurred, has value, and other data	
	/// <summary>
	/// Called when this target recieves a message
	/// </summary>
	void recieve(const std::string& rCommand, const sf::Packet& rData);
	const std::string& getName() const;//name of this
	int getPosition() const;//position of this io component in the list.

protected:
private:
	IOManager* m_pManager;//a convienent reference to our manager
	std::string m_name;//our IO name
	int m_ioManPosition;//position of us in the io manager

	Eventer m_eventer;//the item that stores our conditions and messages
	std::function<void(std::string, sf::Packet)> m_cbFunction;//the function we call when we get a receive message
};

#endif // IOCOMPONENT_HPP
