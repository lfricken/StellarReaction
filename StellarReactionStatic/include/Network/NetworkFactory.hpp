#ifndef NETWORKFACTORY_HPP
#define NETWORKFACTORY_HPP

#include "stdafx.hpp"
#include "NonCopyable.hpp"

class NetworkComponent;

class NetworkFactory : NonCopyable
{
public:
	NetworkFactory(std::string name);
	virtual ~NetworkFactory();

	int give(NetworkComponent* pComponent);
	void clean();//clean up everything
	void free(int position);//don't adjust the list, just mark the node as null and offer it as a position to future components
	void getComponentData(sf::Packet& rPacket);
	void process(sf::Packet& rPacket);

protected:
private:
	std::vector<NetworkComponent*> m_componentPtrs;//where we store all of them
	int m_lastSendID;

	std::string m_name;
};

#endif // NETWORKFACTORY_HPP
