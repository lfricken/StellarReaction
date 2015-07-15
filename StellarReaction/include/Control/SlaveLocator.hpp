#ifndef SLAVELOCATOR_HPP
#define SLAVELOCATOR_HPP

#include "stdafx.hpp"

class Chunk;

class SlaveLocator
{
public:
	SlaveLocator();
	virtual ~SlaveLocator();

	int give(Chunk* pSlave);//we recieve a pointer to a component and we store it and remember the name and position
	void free(int position);//don't adjust the list, just mark the node as null and offer it as a position to future customers
	int findPos(const std::string& slaveTarget);
	Chunk* find(int position);

	Chunk* findHack(const std::string& rName);///what is this???

protected:
private:
	std::vector<Chunk*> m_slavePtrs;//where we store all of them
	std::vector<int> m_freeIndexes;//spots where the ptr is null and we can give a position out!
	std::map<std::string, int> m_nameLookup;//where we lookup the position of an IOComponent after we are told its name
};

#endif // SLAVELOCATOR_HPP
