#pragma once

#include "Courier.hpp"

class IOManager;

/// Stores Conditions and messages to send when those events occur.
class Eventer
{
public:
	/// Eventer takes a reference to an IOManager as a constructor
	/// because the eventer needs to know where to send message that are triggered.
	Eventer(IOManager& rManager);
	virtual ~Eventer();
	/// Stores the Courier so we can check events on it later.
	void add(const Courier& rCourier);
	/// Stores multiple Couriers so we can call events on them later.
	void add(const List<Courier>& rCourierList);
	/// Called when an event happens, and we check to see if any of our conditions are met.
	/// If they are met, we send the corresponding message.
	void event(EventType type, int value, const sf::Packet& rData);
	/// <summary>
	/// Remove all handlers.
	/// </summary>
	void clear();
protected:
private:
	std::map<EventType, List<Courier> > m_courierMap;//holds all the couriers for this object
	IOManager& m_rIOManager;//reference to our manager
};

