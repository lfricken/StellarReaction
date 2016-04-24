#ifndef CONDITION_HPP
#define CONDITION_HPP

#include "EventTypes.hpp"

/*
* Condition Class:
* Determines whether a value should trigger a message to be sent.
*/
class Condition
{
public:
	Condition();
	Condition(EventType type, int value, char comparison, bool repeatable);
	~Condition();
	/// Resets the Condition object to have the given type, value, comparison, and repeatability.
	void reset(EventType type, int value, char comparison, bool repeatable);
	/// Returns true if value satisfies the condition.
	bool evaluate(int value) const;
	/// Return the event type.
	EventType getEventType() const;
	/// Return the comparison of values does this condition do?
	char getComparison() const;
	/// Can this condition be triggered more than once?
	bool isRepeatable() const;

private:

	EventType m_eventType;//type of event that we should check on
	int m_value;//value to compare to (if (newHealth) m_op (m_value)) send message
	char m_op;//operator to determine whether the condition occurs. See cpp for posibilities.
	bool m_isRepeatable;//if this condition sends a message, should we allow it to happen again?
};

#endif // CONDITION_HPP
