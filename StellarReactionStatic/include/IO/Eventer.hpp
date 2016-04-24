#ifndef EVENTER_HPP
#define EVENTER_HPP

#include "Courier.hpp"

class IOManager;

/*
* Eventer Class:
* Stores Conditions and messages to send when those events occur.
*/
class Eventer
{
public:
	Eventer(IOManager& rManager);//when we create an Eventer, we need need to tell it who to send the messages to
	virtual ~Eventer();
	/// Stores the Courier so we can call events on it later.
	void add(const Courier& rCourier);
	/// Stores multiple Couriers so we can call events on them later.
	void add(const std::vector<Courier>& rCourierList);
	/// Called when an event happens, and we check to see if any of our conditions are met;
	/// if they are met, we send the corresponding message.
	void event(EventType type, int value, const sf::Packet& rData);

protected:
private:
	std::map<EventType, std::vector<Courier> > m_courierMap;//holds all the couriers for this object
	IOManager& m_rIOManager;//reference to our manager
};

#endif // EVENTER_HPP
