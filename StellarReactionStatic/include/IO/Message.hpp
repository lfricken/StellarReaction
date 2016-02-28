#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "stdafx.hpp"

/// <summary>
/// Message which is sent to IOComponents
/// Contains a target, either a name or an IOComponent Position,
/// a command, which the object uses to determine what the intended action was
/// a data packet for any additional data to be sent with the action
/// a delay, so the message may not be recieved immediately
/// a boolean to determine whether the data packet should contain Run Time Information <--use false unless you know what you are doing
/// </summary>
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

	void setData(const sf::Packet& rData);
	void setName(const std::string rName);

	void sendOverNW(bool shouldSend);
	bool sendOverNW() const;

	unsigned getTargetPosition() const;
	const std::string& getTargetName() const;
	const std::string& getCommand() const;
	const sf::Packet& getData() const;
	float getDelay() const;

	bool m_replaceTargetPos;//true if we should replace our data with the data sent by the calling object

protected:
private:
	bool m_shouldSendOverNW;//if true, as a client we should send it to our host
	unsigned m_targetPosition;
	std::string m_targetName;//the name of our target IOComponent
	std::string m_command;//the command that we want the target to do
	sf::Packet m_data;//the data associated with that command
	float m_delay;//the delay in seconds that we wait in the IOManager before being delivered

	bool m_replaceData;//true if we should replace our data with the data sent by the calling object
};

#endif // MESSAGE_HPP