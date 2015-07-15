#ifndef IOMANAGER_HPP
#define IOMANAGER_HPP

#include "Timer.hpp"
#include "Courier.hpp"
class IOComponent;

class IOManager
{
public:
	IOManager(bool acceptsLocalMessages);
	virtual ~IOManager();

	/**SEND/RECIEVE MESSAGES**/
	void recieve(const Message& rMessage);//called by a class to give the package to us
	void update(float dT);//iterate over the list of Packages, and if the time is up, call universe.send(Package); on that package
	void toggleAcceptsLocal(bool acceptsLocal);

	/**STORE/FREE COMPONENTS**/
	int give(IOComponent* pComponent);//we recieve a pointer to a component and we store it and remember the name and position
	void free(int position);//don't adjust the list, just mark the node as null and offer it as a position to future components

protected:
private:
	void f_recieveNetwork(const Message& rMessage);
	void f_send(const Message& rMessage);
	bool m_acceptsLocal;//if false, we ignore messages coming to receive function

	std::vector<Message> m_messageList;
	std::vector<IOComponent*> m_componentPtrs;//where we store all of them
	std::vector<int> m_freeIndexes;//spots where the ptr is null and we can give a position out!
	std::map<std::string, int> m_nameLookup;//where we lookup the position of an IOComponent after we are told its name
};

#endif // IOMANAGER_HPP




