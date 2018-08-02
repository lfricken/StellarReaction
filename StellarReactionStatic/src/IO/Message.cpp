#include "Message.hpp"
#include "Globals.hpp"
#include "IOManager.hpp"
#include "Universe.hpp"

/// <summary>
/// Send messages that should get networked, like damage.
/// </summary>
void Message::SendUniverse(const Message& mes)
{
	getGame()->getUniverse().getUniverseIO().recieve(mes);
}

Message::Message()
{
	m_targetPosition = -1;
	m_replaceTargetPos = false;
	m_shouldSendOverNW = false;
}
Message::Message(const String& rTargetName, const String& rCommand, const sf::Packet& rData, float delay, bool replaceData)
{
	reset(rTargetName, rCommand, rData, delay, replaceData);
	m_replaceTargetPos = false;
	m_shouldSendOverNW = false;
}
Message::Message(unsigned rTargetPosition, const String& rCommand, const sf::Packet& rData, float delay, bool replaceData)
{
	reset(rTargetPosition, rCommand, rData, delay, replaceData);
	m_replaceTargetPos = false;
	m_shouldSendOverNW = false;
}
Message::~Message()
{

}
void Message::reset(const String& rTargetName, const String& rCommand, const sf::Packet& rData, float delay, bool replaceData)
{
	m_targetPosition = -1;
	m_targetName = rTargetName;
	m_command = rCommand;
	m_data = rData;
	m_delay = delay;
	m_replaceData = replaceData;
}
void Message::reset(unsigned rTargetPosition, const String& rCommand, const sf::Packet& rData, float delay, bool replaceData)
{
	m_targetPosition = rTargetPosition;
	m_targetName = "";
	m_command = rCommand;
	m_data = rData;
	m_delay = delay;
	m_replaceData = replaceData;
}
void Message::tryReplaceData(const sf::Packet& rData)
{
	if(m_replaceData)
		m_data = rData;
}
void Message::tryReplaceTargetPos(const sf::Packet& rData)
{
	if(m_replaceTargetPos)
	{
		sf::Packet copy(rData);
		int ioPos;
		copy >> ioPos;
		m_targetPosition = ioPos;
	}
}
void Message::changeDelay(float change)
{
	m_delay += change;
}
unsigned Message::getTargetPosition() const
{
	return m_targetPosition;
}
const String& Message::getTargetName() const
{
	return m_targetName;
}
const String& Message::getCommand() const
{
	return m_command;
}
const sf::Packet& Message::getData() const
{
	return m_data;
}
float Message::getDelay() const
{
	return m_delay;
}
void Message::setData(const sf::Packet& rData)
{
	m_data = sf::Packet(rData);
}
void Message::setName(const String rName)
{
	m_targetName = rName;
}
void Message::sendOverNW(bool shouldSend)
{
	m_shouldSendOverNW = shouldSend;
}
bool Message::sendOverNW() const
{
	return m_shouldSendOverNW;
}
