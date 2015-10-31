#ifndef IOMANAGER_HPP
#define IOMANAGER_HPP

#include "Timer.hpp"
#include "Courier.hpp"

class NetworkComponent;
class IOComponent;

/// <summary>
/// Controlls the sending of Messages throughout the program
/// </summary>
class IOManager
{
public:
	IOManager(bool acceptsLocalMessages, bool networked = false);
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
	void f_send(const Message& rMessage);
	bool m_acceptsLocal;//if false, we ignore messages coming to receive function

	void pack(sf::Packet& rPacket);//give us data to send to the twin in the other world
	void unpack(sf::Packet& rPacket);//process data from our twin

	sptr<NetworkComponent> m_spNw;

	std::vector<Message> m_latest;

	std::vector<Message> m_messageList;
	std::vector<IOComponent*> m_componentPtrs;//where we keep a pointer to all IOCompontents
	std::vector<int> m_freeIndexes;//spots where the ptr is null and we can give a position out!
	std::map<std::string, int> m_nameLookup;//where we lookup the position of an IOComponent after we are told its name

	bool m_networked;
};

#endif // IOMANAGER_HPP




