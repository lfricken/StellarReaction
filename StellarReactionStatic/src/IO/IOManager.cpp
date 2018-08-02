#include "IOManager.hpp"
#include "stdafx.hpp"
#include "Globals.hpp"
#include "IOComponent.hpp"
#include "NetworkBoss.hpp"
#include "NetworkComponent.hpp"
#include "Debugging.hpp"

IOManager::IOManager(bool acceptsLocalMessages, bool networked)
{
	m_networked = networked;
	if(m_networked)
	{
		m_spNw = sptr<NetworkComponent>(new NetworkComponent(NetworkComponentData(), &IOManager::pack, &IOManager::unpack, this, getGame()->getNwBoss().getNWFactoryTcp()));
	}

	m_acceptsLocal = acceptsLocalMessages;
}
IOManager::~IOManager()
{

}
void IOManager::recieve(const Message& rMessage)
{
	if(m_networked && rMessage.sendOverNW())//we are someone about to send an important message
	{
		m_spNw->toggleNewData(true);
		m_latest.push_back(rMessage);
	}
	else if(m_networked && getGame()->getNwBoss().getNWState() == NWState::Server)//we are the host with any info
	{
		m_spNw->toggleNewData(true);
		m_latest.push_back(rMessage);
	}

	if(m_acceptsLocal)
	{
		m_messageList.push_back(rMessage);//if we accept local, otherwise ignore it
	}
}
void IOManager::update(float dT)
{
	const int maxWork = 500;//prevents infinite loops of messages, or too much cpu time spent here per frame
	for(int i = 0; i < (signed)m_messageList.size(); ++i)
	{
		m_messageList[i].changeDelay(-dT);

		if(m_messageList[i].getDelay() <= 0)
		{
			f_send(m_messageList[i]);
			m_messageList.erase(m_messageList.begin() + i);
			--i;
		}
		if(i >= maxWork)
		{
			WARNING;
			/// ERROR
			break;
		}
	}
}
void IOManager::toggleAcceptsLocal(bool acceptsLocal)
{
	m_acceptsLocal = acceptsLocal;
}
int IOManager::give(IOComponent* pComponent)//we recieve a pointer to a component and we store it and remember the name and position
{
	int position;

	if(!m_freeIndexes.empty())//check free positions
	{
		position = m_freeIndexes.back();
		m_freeIndexes.pop_back();
	}
	else//assign new position at end
	{
		position = m_componentPtrs.size();
		m_componentPtrs.resize(m_componentPtrs.size() + 1);//add one
	}

	m_componentPtrs[position] = pComponent;

	m_nameLookup[pComponent->getName()] = position;
	
	return position;
}
void IOManager::freeThis(int position)//don't adjust the list, just mark the node as null and offer it as a position to future customers
{
	if((signed)m_componentPtrs.size() > position)
	{
		m_componentPtrs[position] = nullptr;
		m_freeIndexes.push_back(position);
		for(auto it = m_nameLookup.begin(); it != m_nameLookup.end(); ++it)
		{
			if(it->second == position)
			{
				m_nameLookup.erase(it);
				break;
			}
		}
	}
	else
	{
		WARNING;
		///ERROR LOG
	}
}
void IOManager::f_send(const Message& rMessage)
{
	int pos = rMessage.getTargetPosition();

	if(rMessage.getTargetName() != "")
	{
		auto it = m_nameLookup.find(rMessage.getTargetName());
		if(it != m_nameLookup.end())
		{
			if(m_componentPtrs[it->second] != nullptr)
			{
				m_componentPtrs[it->second]->recieve(rMessage.getCommand(), rMessage.getData());
			}
			else
			{
				Print << "\nTarget: [" << rMessage.getTargetName() << "] died. ";
			}
		}
		else
		{
			Print << "\nTarget: [" << rMessage.getTargetName() << "] was not found.";
		}
	}
	else if(pos >= 0 && pos < (signed)m_componentPtrs.size())
	{
		if(m_componentPtrs[pos] != nullptr)
			m_componentPtrs[pos]->recieve(rMessage.getCommand(), rMessage.getData());
	}
	else
	{
		WARNING;
	}
}
void IOManager::pack(sf::Packet& rPacket)//give us data to send to the twin in the other world
{
	int32_t totalSize = m_latest.size(); // total number of messages
	rPacket << totalSize;
	for(int32_t i = 0; i < (signed)m_latest.size(); ++i)
	{
		rPacket << (int32_t)m_latest[i].getTargetPosition();
		rPacket << m_latest[i].getTargetName();
		rPacket << m_latest[i].getCommand();

		int dataSize = (signed)m_latest[i].getData().getDataSize();

		rPacket << (int32_t)dataSize;

		int8_t* pByte = (int8_t*)m_latest[i].getData().getData();
		for(int index = 0; index < dataSize; ++index)
		{
			int8_t byte = pByte[index];
			rPacket << byte;
		}

		rPacket << m_latest[i].getDelay();
	}

	m_latest.clear();
}
void IOManager::unpack(sf::Packet& rPacket)//process data from our twin
{
	int32_t totalNumber; // total number of messages
	rPacket >> totalNumber;
	for(int32_t i = 0; i < totalNumber; ++i)
	{
		int32_t pos;
		String name;
		String command;
		int32_t sizeInnerPacket; // size of the sf::Packet inside the message
		int8_t* pData;
		sf::Packet messageData;
		float delay;

		rPacket >> pos;
		rPacket >> name;
		rPacket >> command;

		rPacket >> sizeInnerPacket;

		pData = new int8_t[sizeInnerPacket];
		for(int index = 0; index < sizeInnerPacket; ++index)
		{
			int8_t byte;
			rPacket >> byte;
			pData[index] = byte;
		}
		messageData.append(pData, sizeInnerPacket);

		rPacket >> delay;

		Message fromNetwork((unsigned int)pos, command, messageData, delay, false);
		fromNetwork.setName(name);//if they sent a name instead of a position, set that instead

		if(getGame()->getNwBoss().getNWState() == NWState::Server) // if we are the server, we should forward it to our clients too!
			recieve(fromNetwork);
		else // if we are our clients, we should just hear it
			m_messageList.push_back(fromNetwork);

		delete[] pData;
	}
}
