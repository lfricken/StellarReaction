#pragma once

#include "EventTypes.hpp"


/// \brief Used to determine whether a value should trigger a message to be sent.
///
/// It is given a comparison operator from the following list, and uses it
/// to evaluate an int when requested. It also can be told to or not to repeat.
/// It evaluates as:
/// checkValue operator m_value
/// '>' for greater than
/// '<' for less than
/// '=' equal
/// '!' not equal
/// 'd' (delta) always trigger
class Condition
{
public:
	Condition();
	Condition(EventType type, int value, char comparison, bool repeatable);
	~Condition();
	/// Resets the Condition object to have the given EventType, value, comparison operator, and whether it can repeat.
	void reset(EventType type, int value, char comparison, bool repeatable);
	/// Returns true if the value satisfies the condition.
	bool evaluate(int value) const;
	/// Return the type of event as EventType.
	EventType getEventType() const;
	/// Return the comparison operator as a char.
	char getComparison() const;
	/// Can this condition be triggered more than once?
	bool isRepeatable() const;

private:

	EventType m_eventType;//type of event that we should check on
	int m_value;//value to compare to (if (newHealth) m_op (m_value)) send message
	char m_op;//operator to determine whether the condition occurs. See cpp for posibilities.
	bool m_isRepeatable;//if this condition sends a message, should we allow it to happen again?
};


