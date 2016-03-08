#ifndef CONDITION_HPP
#define CONDITION_HPP

#include "EventTypes.hpp"

/// <summary>
/// Determines whether a value should trigger a message to be sent.
/// </summary>
class Condition
{
public:
	Condition();
	Condition(EventType type, int value, char comparison, bool repeatable);
	~Condition();
	void reset(EventType type, int value, char comparison, bool repeatable);

	bool evaluate(int value) const;//returns true if value satisfies the condition

	EventType getEventType() const;//What type of event are we for?
	char getComparison() const;//what type of comparison of values does this condition do?
	bool isRepeatable() const;//can this condition be triggered only once?

private:

	EventType m_eventType;//type of event that we should check on
	int m_value;//value to compare to (if (newHealth) m_op (m_value)) send message
	char m_op;//operator to determine whether the condition occurs. See cpp for posibilities.
	bool m_isRepeatable;//if this condition sends a message, should we allow it to happen again?
};

#endif // CONDITION_HPP
