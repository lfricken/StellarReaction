#include "SlaveLocator.hpp"

#include "Globals.hpp"
#include "Chunk.hpp"

using namespace std;

SlaveLocator::SlaveLocator()
{

}
SlaveLocator::~SlaveLocator()
{

}
int SlaveLocator::give(Chunk* pSlave)//we recieve a pointer to a component and we store it and remember the name and position
{
	int position;

	if(!m_freeIndexes.empty())//check free positions
	{
		position = m_freeIndexes.back();
		m_freeIndexes.pop_back();
	}
	else//assign new position at end
	{
		position = m_slavePtrs.size();
		m_slavePtrs.resize(m_slavePtrs.size()+1);//add one
	}

	m_slavePtrs[position] = pSlave;

	string name = pSlave->getName();
	m_nameLookup[name] = position;

	return position;
}
void SlaveLocator::free(int position)//don't adjust the list, just mark the node as null and offer it as a position to future users
{
	if((signed)m_slavePtrs.size() > position)
	{
		m_slavePtrs[position] = NULL;
		m_freeIndexes.push_back(position);
	}
	else
	{
		cout << FILELINE;
		///ERROR LOG
	}
}
int SlaveLocator::findPos(const std::string& slaveTarget)
{
	auto it = m_nameLookup.find(slaveTarget);
	if(it != m_nameLookup.end())
	{
		return it->second;
	}
	else
	{
		cout << "\nTarget [" << slaveTarget << "] was not found." << FILELINE;
		///ERROR LOG
		return 0;
	}
}
Chunk* SlaveLocator::find(int position)
{
	if((signed)m_slavePtrs.size() > position && position >= 0)
		return m_slavePtrs[position];
	else
		return NULL;
}
Chunk* SlaveLocator::find(const std::string& rName)
{
	int index = findPos(rName);
	if(index == -1)
		return find(index);
	else
		return NULL;
}
