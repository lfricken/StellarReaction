#ifndef EVENTER_HPP
#define EVENTER_HPP

#include "Courier.hpp"

class IOManager;

class Eventer
{
public:
	Eventer(IOManager& rManager);//when we create an Eventer, we need need to tell it who to send the messages to
	virtual ~Eventer();

	void add(const Courier& rCourier);//gets sent a courier and puts it into its vector to be checked later
	void add(const std::vector<Courier>& rCourierList);

	void event(EventType type, int value, const sf::Packet& rData);

protected:
private:
	std::map<EventType, std::vector<Courier> > m_courierMap;/**holds all the couriers for this object**/
	IOManager& m_rIOManager;
};

#endif // EVENTER_HPP
