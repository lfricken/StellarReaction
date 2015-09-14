#include "IOManager.hpp"
#include "stdafx.hpp"
#include "Globals.hpp"
#include "IOComponent.hpp"
#include "NetworkBoss.hpp"
#include "NetworkComponent.hpp"

using namespace std;

IOManager::IOManager(bool acceptsLocalMessages, bool networked)
{
	m_networked = networked;
	if(m_networked)
		m_spNw = sptr<NetworkComponent>(new NetworkComponent(NetworkComponentData(), &IOManager::pack, &IOManager::unpack, this, game.getNwBoss().getNWFactoryTcp()));
	m_acceptsLocal = acceptsLocalMessages;
}
IOManager::~IOManager()
{

}
void IOManager::recieve(const Message& rMessage)
{
	if(m_acceptsLocal)
	{
		if(m_networked && rMessage.sendOverNW())//we are someone about to send an important message
		{
			m_spNw->toggleNewData(true);
			m_latest.push_back(rMessage);
		}
		else if(m_networked && game.getNwBoss().getNWState == NWState::Server)//we are the host with any info
		{
			m_spNw->toggleNewData(true);
			m_latest.push_back(rMessage);
		}
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
			cout << FILELINE;
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
void IOManager::free(int position)//don't adjust the list, just mark the node as null and offer it as a position to future customers
{
	if((signed)m_componentPtrs.size() > position)
	{
		m_componentPtrs[position] = NULL;
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
		cout << FILELINE;
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
			if(m_componentPtrs[it->second] != NULL)
				m_componentPtrs[it->second]->recieve(rMessage.getCommand(), rMessage.getData());
			else
			{
				cout << "\nHe's dead Jim. [" << rMessage.getTargetName() << "]." << FILELINE;
				///ERROR LOG
			}
		}
		else
		{
			cout << "\nTarget [" << rMessage.getTargetName() << "] was not found." << FILELINE;
			///ERROR LOG
		}
	}
	else if(pos >= 0 && pos < (signed)m_componentPtrs.size())
	{
		if(m_componentPtrs[pos] != NULL)
			m_componentPtrs[pos]->recieve(rMessage.getCommand(), rMessage.getData());
	}
	else
	{
		cout << "\nTarget [" << rMessage.getTargetName() << "][" << pos << "]." << FILELINE;
		///ERROR LOG
	}
}
void IOManager::pack(sf::Packet& rPacket)//give us data to send to the twin in the other world
{
	if(m_acceptsLocal)
	{
		int32_t total = m_latest.size();
		rPacket << total;
		for(int32_t i = 0; i < (signed)m_latest.size(); ++i)
		{
			rPacket << (int32_t)m_latest[i].getTargetPosition();
			rPacket << m_latest[i].getTargetName();
			rPacket << m_latest[i].getCommand();

			int dataSize = (signed)m_latest[i].getData().getDataSize();

			rPacket << (int32_t)dataSize;

			int8_t* pByte = (int8_t*)m_latest[i].getData().getData();
			for(int chard = 0; chard < dataSize; ++chard)
			{
				int8_t byte = pByte[chard];
				rPacket << byte;
			}

			rPacket << m_latest[i].getDelay();
		}
	}
	m_latest.clear();
}
void IOManager::unpack(sf::Packet& rPacket)//process data from our twin
{
	if(!m_acceptsLocal)
	{
		int32_t total;
		rPacket >> total;
		for(int32_t i = 0; i < total; ++i)
		{
			int32_t pos;
			std::string name;
			std::string command;
			int32_t size;
			int8_t* pData;
			sf::Packet messageData;
			float delay;

			rPacket >> pos;
			rPacket >> name;
			rPacket >> command;

			rPacket >> size;

			pData = new int8_t[size];
			for(int chard = 0; chard < size; ++chard)
			{
				int8_t byte;
				rPacket >> byte;
				pData[chard] = byte;
			}
			messageData.append(pData, size);

			rPacket >> delay;

			m_messageList.push_back(Message((unsigned int)pos, command, messageData, delay, false));
			m_messageList.back().setName(name);
			delete pData;
		}
	}
}
