#include "IOManager.hpp"
#include "stdafx.hpp"
#include "Globals.hpp"
#include "IOComponent.hpp"
#include "NetworkBoss.hpp"

using namespace std;

IOManager::IOManager(bool acceptsLocalMessages)
{
	m_acceptsLocal = acceptsLocalMessages;
}
IOManager::~IOManager()
{

}
/**SEND/RECIEVE MESSAGES**/
void IOManager::recieve(const Message& rMessage)//called by a class to give the package to us
{
	if(m_acceptsLocal)
		m_messageList.push_back(rMessage);//if we accept local, otherwise ignore it
}
void IOManager::f_recieveNetwork(const Message& rMessage)
{
	m_messageList.push_back(rMessage);
}
void IOManager::update(float dT)//iterate over the list of Packages, and if the time is up, call universe.send(Package); on that package
{

	const int maxWork = 2000;//prevents infinite loops of messages, or too much cpu time spent here per frame
	for(int i = 0; i<(signed)m_messageList.size(); ++i)
	{
		m_messageList[i].changeDelay(-dT);

		if(m_messageList[i].getDelay() <= 0)
		{
			f_send(m_messageList[i]);
			m_messageList.erase(m_messageList.begin()+i);
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

/**STORE/FREE COMPONENTS**/
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
		m_componentPtrs.resize(m_componentPtrs.size()+1);//add one
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
