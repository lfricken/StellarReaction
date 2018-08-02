#include "NetworkFactory.hpp"
#include "Globals.hpp"
#include "NetworkComponent.hpp"
#include "Protocol.hpp"
#include "Debugging.hpp"

NetworkFactory::NetworkFactory(String name)
{
	m_name = name;
	m_lastSendID = 0;
	m_consecutiveDesyncs = 0;
}
NetworkFactory::~NetworkFactory()
{

}
int NetworkFactory::give(NetworkComponent* pComponent)//we recieve a pointer to a component and we store it
{

	int position;

	position = m_componentPtrs.size();
	m_componentPtrs.resize(m_componentPtrs.size() + 1);//add one

	m_componentPtrs[position] = pComponent;
	return position;
}
void NetworkFactory::freeThis(int position)//don't adjust the list, just mark the node as null and offer it as a position to future customers
{
	if((signed)m_componentPtrs.size() > position)
		m_componentPtrs[position] = nullptr;
	else
	{
		WARNING;
	}
	clean();
}
void NetworkFactory::clean()
{
	while(m_componentPtrs.size() > 0)
	{
		if(m_componentPtrs.back() == nullptr)
			m_componentPtrs.resize(m_componentPtrs.size() - 1);
		else
			break;
	}
}
/// <summary>
/// gets data of all NetworkComponents. Stored as number of components, then a list of the ID of component, then the data it sends/recieves
/// </summary>
void NetworkFactory::getComponentData(sf::Packet& rPacket)
{
	rPacket << (int32_t)m_componentPtrs.size();
	List<NetworkComponent*>& rPtr = m_componentPtrs;
	for(int32_t i = 0; i < (signed)rPtr.size(); ++i)
	{
		if(rPtr[i] != nullptr)
		{
			if(rPtr[i]->hasNewData())
			{
				rPacket << i;
				rPtr[i]->pack(rPacket);
				rPtr[i]->toggleNewData(false);
			}
		}
	}
}
void NetworkFactory::process(sf::Packet& rPacket)
{
	int32_t numElements;
	rPacket >> numElements;
	int32_t expectedNumElements = (int32_t)m_componentPtrs.size();
	if(numElements == expectedNumElements)
	{
		m_consecutiveDesyncs = 0;
		int32_t id = 0;
		int32_t old_id = 0;//for debugging purposes remembers the last attempted id process
		while(rPacket >> id && !rPacket.endOfPacket())
		{
			if(id < (signed)m_componentPtrs.size())
			{
				if(m_componentPtrs[id] != nullptr)//crashing here because the controller is dead but we recieved a packet meant for it
				{
					m_componentPtrs[id]->unpack(rPacket);//for each component id, assume it will extract the appropriate amount of data from the packet
					//so the next extract of the id will result in a correct id.
				}
				else
					break;//stop because next id will be garbage since we couldn't extract the data, this packet is toast
			}
			else
			{
				break;//stop because next id will be garbage since we couldn't extract the data, this packet is toast
			}
			old_id = id;
		}
	}
	else
	{
		m_consecutiveDesyncs++;
		int i = 9;
	}
	if(m_consecutiveDesyncs > 0 && m_consecutiveDesyncs > 10)
	{
		m_consecutiveDesyncs = 0;
		Print << "\n10 Desyncs Detected.";
	}
}
