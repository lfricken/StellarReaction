#pragma once

#include "stdafx.hpp"

/// \brief Message which is sent to IOComponents.
///
/// Contains a target, either a name or an IOComponent Position,
/// a command, which the target object uses to determine what the intended action was
/// a data packet for any additional data to be sent with the action
/// a delay, so the message may not be recieved immediately
/// a boolean to determine whether the data packet should contain Run Time Information (use false unless you know what you are doing)
class Message
{
public:
	static void SendUniverse(const Message& mes);

	Message();
	Message(const String& rTargetName, const String& rCommand, const sf::Packet& rData, float delay, bool replaceData);
	Message(unsigned rTargetPosition, const String& rCommand, const sf::Packet& rData, float delay, bool replaceData);
	virtual ~Message();
	/// Resets the Message object with given target name, command, and packet info.
	void reset(const String& rTargetName, const String& rCommand, const sf::Packet& rData, float delay, bool replaceData);
	/// Resets the Message object with given target position, command, and packet info.
	void reset(unsigned rTargetPosition, const String& rCommand, const sf::Packet& rData, float delay, bool replaceData);
	/// Attempt to replace the target position's with packet's target position.
	void tryReplaceTargetPos(const sf::Packet& rData);
	/// Attempt to replace the data with this packet.
	/// If this message has the replaceData flag set to true,
	/// it will accept the data.
	void tryReplaceData(const sf::Packet& rData);
	/// Change delay.
	void changeDelay(float change);
	/// Sets data.
	void setData(const sf::Packet& rData);
	/// Set name.
	void setName(const String rName);
	/// <summary>
	/// Should this message be sent over network even if we are a Client?
	/// It will be sent if we are the Server no matter what.
	/// Used for
	/// </summary>
	void sendOverNW(bool shouldSend);
	/// <summary>
	/// Should this message be sent over network even if we are a Client?
	/// It will be sent if we are the Server no matter what.
	/// </summary>
	bool sendOverNW() const;
	/// Returns targetPosition.
	unsigned getTargetPosition() const;
	/// Returns targetName.
	const String& getTargetName() const;
	/// Returns command.
	const String& getCommand() const;
	/// Returns data.
	const sf::Packet& getData() const;
	/// Returns the delay.
	float getDelay() const;

	/// True if we should replace our data with the data sent by the calling object.
	bool m_replaceTargetPos;

protected:
private:
	bool m_shouldSendOverNW;//if true, as a client we should send it to our host
	unsigned m_targetPosition;
	String m_targetName;//the name of our target IOComponent
	String m_command;//the command that we want the target to do
	sf::Packet m_data;//the data associated with that command
	float m_delay;//the delay in seconds that we wait in the IOManager before being delivered

	bool m_replaceData;//true if we should replace our data with the data sent by the calling object
};

