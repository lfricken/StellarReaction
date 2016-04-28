#pragma once

#include "Timer.hpp"
#include "Courier.hpp"
#include "NonCopyable.hpp"

class NetworkComponent;
class IOComponent;


/// Controls the sending of Messages throughout the program and stores all the IOComponents in a list.
class IOManager : NonCopyable
{
public:
	IOManager(bool acceptsLocalMessages, bool networked = false);
	virtual ~IOManager();

	/// Put a message in here to have it sent.
	void recieve(const Message& rMessage);
	/// Check if any packages should be sent, since dT time has passed.
	void update(float dT);
	/// \brief Toggles whether messages coming from local items should be processed or not.
	///
	/// If we are the client, we should ignore local events.
	void toggleAcceptsLocal(bool acceptsLocal);

	/// Recieves a pointer to a component and stores it and remember the name and position.
	int give(IOComponent* pComponent);
	/// Marks the position as null and opens it for other IOComponents.
	void free(int position);

protected:
private:
	/// Call to send a message.
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


