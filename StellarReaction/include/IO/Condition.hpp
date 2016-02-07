#ifndef CONDITION_HPP
#define CONDITION_HPP

#include "EventTypes.hpp"

/// <summary>
/// determines whether a value 
/// 
/// 
/// TODO This is unnecessarily complex, dont use function pointers
/// </summary>
class Condition
{
public:
	Condition();
	Condition(EventType type, int value, char comparison, bool repeatable);
	~Condition();
	void reset(EventType type, int value, char comparison, bool repeatable);


	EventType getEventType() const;
	bool evaluate(int value) const;//returns true if in value satisfies the condition
	char getComparison() const;
	bool isRepeatable() const;

private:
	typedef bool (Condition::*ComparisonFunction)(int) const;

	bool f_greaterThan(int value) const;
	bool f_lessThan(int value) const;
	bool f_equals(int value) const;+
	bool f_notEquals(int value) const;
	bool f_change(int value) const;

	void f_setComparisonFunction(char op);
	ComparisonFunction m_evaluationFunction;

	EventType m_eventType;//type of event that we should check on
	int m_value;//in case its a numerical value, we can put it in here when this is created
	char m_comparison;//character type used to compare, so it could be >, <, or =
	bool m_isRepeatable;//used by the Eventer to decide whether to keep this Courier after it got activated once
};

#endif // CONDITION_HPP
