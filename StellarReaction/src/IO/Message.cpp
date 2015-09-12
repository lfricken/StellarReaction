#include "Message.hpp"

Message::Message()
{
	m_targetPosition = -1;
	m_replaceTargetPos = false;
}
Message::Message(const std::string& rTargetName, const std::string& rCommand, const sf::Packet& rData, float delay, bool replaceData)
{
	reset(rTargetName, rCommand, rData, delay, replaceData);
	m_replaceTargetPos = false;
}
Message::Message(unsigned rTargetPosition, const std::string& rCommand, const sf::Packet& rData, float delay, bool replaceData)
{
	reset(rTargetPosition, rCommand, rData, delay, replaceData);
	m_replaceTargetPos = false;
}
Message::~Message()
{

}
void Message::reset(const std::string& rTargetName, const std::string& rCommand, const sf::Packet& rData, float delay, bool replaceData)
{
	m_targetPosition = -1;
	m_targetName = rTargetName;
	m_command = rCommand;
	m_data = rData;
	m_delay = delay;
	m_replaceData = replaceData;
}
void Message::reset(unsigned rTargetPosition, const std::string& rCommand, const sf::Packet& rData, float delay, bool replaceData)
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
const std::string& Message::getTargetName() const
{
	return m_targetName;
}
const std::string& Message::getCommand() const
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