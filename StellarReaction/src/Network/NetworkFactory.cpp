#include "NetworkFactory.hpp"
#include "Globals.hpp"
#include "NetworkComponent.hpp"
#include "Protocol.hpp"

using namespace std;


NetworkFactory::NetworkFactory(std::string name)
{
	m_name = name;
	m_lastSendID = 0;
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
void NetworkFactory::free(int position)//don't adjust the list, just mark the node as null and offer it as a position to future customers
{
	if((signed)m_componentPtrs.size() > position)
		m_componentPtrs[position] = NULL;
	else
	{
		cout << position << m_name;
		cout << m_componentPtrs.size();
		cout << FILELINE;
		///ERROR LOG
	}
	clean();
}
void NetworkFactory::clean()
{
	while(m_componentPtrs.size() > 0)
	{
		if(m_componentPtrs.back() == NULL)
			m_componentPtrs.resize(m_componentPtrs.size() - 1);
		else
			break;
	}
}
/// <summary>
/// gets data of all NetworkComponents
/// </summary>
void NetworkFactory::getComponentData(sf::Packet& rPacket)
{
	std::vector<NetworkComponent*>& rPtr = m_componentPtrs;
	for(int32_t i = 0; i < (signed)rPtr.size(); ++i)
	{
		if(rPtr[i] != NULL)
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
	int32_t id;
	int32_t old_id;
	while(rPacket >> id && !rPacket.endOfPacket())
	{
		old_id = id;
		if(id < (signed)m_componentPtrs.size())
		{
			if(m_componentPtrs[id] != NULL)
			{
				m_componentPtrs[id]->unpack(rPacket);
			}
		}
		else
		{
			cout << "\n[" << id << "][" << old_id << "]" << FILELINE << m_name;
			///ERROR LOG
			break;
		}
	}
}
