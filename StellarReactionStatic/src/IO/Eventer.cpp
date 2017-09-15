#include "Eventer.hpp"
#include "IOManager.hpp"

Eventer::Eventer(IOManager& rManager) : m_rIOManager(rManager)
{

}
Eventer::~Eventer()
{

}
void Eventer::add(const Courier& rCourier)//gets sent a courier and puts it into its List to be checked later
{
	m_courierMap[rCourier.condition.getEventType()].push_back(rCourier);
}
void Eventer::add(const List<Courier>& rCourierList)
{
	for(auto it = rCourierList.cbegin(); it!=rCourierList.cend(); ++it)
		add(*it);
}
void Eventer::clear()
{
	m_courierMap.clear();
}
void Eventer::event(EventType type, int value, const sf::Packet& rData)
{
	auto typeIt = m_courierMap.find(type);//find the expected type of event

	if(typeIt != m_courierMap.end())
	{
		for(int i = 0; i<(signed)typeIt->second.size(); ++i)//each courier of that type
		{
			List<Courier>& rList = typeIt->second;

			if(rList[i].condition.evaluate(value))
			{
				Message copy(rList[i].message);
				copy.tryReplaceTargetPos(rData);
				copy.tryReplaceData(rData);
				m_rIOManager.recieve(copy);

				if(!rList[i].condition.isRepeatable())//if this message shouldn't repeat
				{
					rList.erase(rList.begin()+i);
					--i;
				}
			}
		}
	}
}
