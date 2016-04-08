#ifndef SLAVELOCATOR_HPP
#define SLAVELOCATOR_HPP

#include "stdafx.hpp"
#include "NonCopyable.hpp"

class Chunk;

/// <summary>
/// This class holds pointers to all the chunks
/// </summary>
class SlaveLocator : NonCopyable
{
public:
	SlaveLocator();
	virtual ~SlaveLocator();

	int give(Chunk* pSlave);//we recieve a pointer to a component and we store it and remember the name and position (return position)
	void free(int position);//don't adjust the list, just mark the node as null and offer it as a position to future customers
	int findPos(const std::string& slaveTarget);//-1 if it can't be found
	Chunk* find(int position);

	Chunk* findHack(const std::string& rName);//TODO Rename; returns chunk pointer by name

protected:
private:
	std::vector<Chunk*> m_slavePtrs;//where we store all of them
	std::vector<int> m_freeIndexes;//spots where the ptr is null and we can give a position out!
	std::map<std::string, int> m_nameLookup;//where we lookup the position of an IOComponent after we are told its name
};

#endif // SLAVELOCATOR_HPP
