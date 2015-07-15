#ifndef IOCOMPONENTSTORE_HPP
#define IOCOMPONENTSTORE_HPP

#include "stdafx.hpp"
class IOComponent;

class IOComponentStore
{
public:
    IOComponentStore();
    virtual ~IOComponentStore();

    int give(IOComponent* pComponent);//we recieve a pointer to a component and we store it and remember the name and position
    void free(int position);//don't adjust the list, just mark the node as null and offer it as a position to future customers

protected:
private:
    std::vector<IOComponent*> m_componentPtrs;//where we store all of them
    std::vector<int> m_freeIndexes;//spots where the ptr is null and we can give a position out!
    std::map<std::string, int> m_nameLookup;//where we lookup the position of an IOComponent after we are told its name
};

#endif // IOCOMPONENTSTORE_HPP
