#ifndef IOMANAGER_HPP
#define IOMANAGER_HPP

#include "Timer.hpp"
#include "Courier.hpp"
class IOComponent;

/// <summary>
/// Controlls the sending of Messages throughout the program
/// </summary>
class IOManager
{
public:
	IOManager(bool acceptsLocalMessages);
	virtual ~IOManager();

	/**SEND/RECIEVE MESSAGES**/
	void recieve(const Message& rMessage);//Put a message in here to have it sent
	void update(float dT);//check if any packages should be sent out after that deltaTime
	/// <summary>
	/// Toggles whether messages coming from local items should be processed or not
	/// if we are in a multiplayer game, and are the client, we should ignore local events
	/// </summary>
	/// <param name="acceptsLocal">if set to.</param>
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
	std::vector<IOComponent*> m_componentPtrs;//where we keep a pointer to all IOCompontents
	std::vector<int> m_freeIndexes;//spots where the ptr is null and we can give a position out!
	std::map<std::string, int> m_nameLookup;//where we lookup the position of an IOComponent after we are told its name
};

#endif // IOMANAGER_HPP




