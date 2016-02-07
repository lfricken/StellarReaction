#ifndef EVENTER_HPP
#define EVENTER_HPP

#include "Courier.hpp"

class IOManager;

/// <summary>
/// Stores Conditions and messages to send when those events occur.
/// </summary>
class Eventer
{
public:
	Eventer(IOManager& rManager);//when we create an Eventer, we need need to tell it who to send the messages to
	virtual ~Eventer();

	void add(const Courier& rCourier);//store the Courrier so we can event on it later
	void add(const std::vector<Courier>& rCourierList);//store the Courriers so we can event on them later		
	/// <summary>
	/// called when an event happens, and we check to see if any of our conditions are met
	/// </summary>
	void event(EventType type, int value, const sf::Packet& rData);

protected:
private:
	std::map<EventType, std::vector<Courier> > m_courierMap;//holds all the couriers for this object
	IOManager& m_rIOManager;//reference to our manager
};

#endif // EVENTER_HPP
