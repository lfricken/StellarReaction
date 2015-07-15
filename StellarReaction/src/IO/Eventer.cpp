#include "Eventer.hpp"
#include "IOManager.hpp"

using namespace std;

Eventer::Eventer(IOManager& rManager) : m_rIOManager(rManager)
{

}
Eventer::~Eventer()
{

}
void Eventer::add(const Courier& rCourier)//gets sent a courier and puts it into its vector to be checked later
{
	m_courierMap[rCourier.condition.getEventType()].push_back(rCourier);
}
void Eventer::add(const std::vector<Courier>& rCourierList)
{
	for(auto it = rCourierList.cbegin(); it!=rCourierList.cend(); ++it)
		add(*it);
}
void Eventer::event(EventType type, int value, const sf::Packet& rData)
{
	auto typeIt = m_courierMap.find(type);

	if(typeIt != m_courierMap.end())
	{
		for(int i = 0; i<(signed)typeIt->second.size(); ++i)//each courier of that type
		{
			vector<Courier>& rList = typeIt->second;

			if(rList[i].condition.evaluate(value))
			{
				Message copy(rList[i].message);
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
