#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "stdafx.hpp"

class Message
{
public:
	Message();
	Message(const std::string& rTargetName, const std::string& rCommand, const sf::Packet& rData, float delay, bool replaceData);
	Message(unsigned rTargetPosition, const std::string& rCommand, const sf::Packet& rData, float delay, bool replaceData);
	virtual ~Message();
	void reset(const std::string& rTargetName, const std::string& rCommand, const sf::Packet& rData, float delay, bool replaceData);
	void reset(unsigned rTargetPosition, const std::string& rCommand, const sf::Packet& rData, float delay, bool replaceData);

	void tryReplaceTargetPos(const sf::Packet& rData);
	void tryReplaceData(const sf::Packet& rData);
	void changeDelay(float change);//the m_delay will be changed by that much (used by IOManager)

	unsigned getTargetPosition() const;
	const std::string& getTargetName() const;
	const std::string& getCommand() const;
	const sf::Packet& getData() const;
	float getDelay() const;

	bool m_replaceTargetPos;//true if we should replace our data with the data sent by the calling object

protected:
private:
	unsigned m_targetPosition;
	std::string m_targetName;//the name of our target IOComponent
	std::string m_command;//the command that we want the target to do
	sf::Packet m_data;//the data associated with that command
	float m_delay;//the delay in seconds that we wait in the IOManager before being delivered

	bool m_replaceData;//true if we should replace our data with the data sent by the calling object
};

#endif // MESSAGE_HPP
