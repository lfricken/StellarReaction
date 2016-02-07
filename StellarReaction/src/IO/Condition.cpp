#include "Condition.hpp"
#include "stdafx.hpp"
#include "Globals.hpp"

using namespace std;

Condition::Condition()
{
	reset(EventType::None, 0, '<', false);
}
Condition::Condition(EventType type, int value, char comparison, bool repeatable)
{
	reset(type, value, comparison, repeatable);
}
Condition::~Condition()
{

}
void Condition::reset(EventType type, int value, char comparison, bool repeatable)
{
	m_eventType = type;
	m_value = value;
	m_comparison = comparison;
	m_isRepeatable = repeatable;
	f_setComparisonFunction(comparison);
}
EventType Condition::getEventType() const
{
	return m_eventType;
}
char Condition::getComparison() const
{
	return m_comparison;
}
bool Condition::isRepeatable() const
{
	return m_isRepeatable;
}
bool Condition::evaluate(int value) const
{
	return (*this.*m_evaluationFunction)(value);
}
void Condition::f_setComparisonFunction(char op)
{
	if (op == '>')
		m_evaluationFunction = &Condition::f_greaterThan;
	else if (op == '<')
		m_evaluationFunction = &Condition::f_lessThan;
	else if (op == '=')
		m_evaluationFunction = &Condition::f_equals;
	else if (op == '!')
		m_evaluationFunction = &Condition::f_notEquals;
	else if (op == 'd')
		m_evaluationFunction = &Condition::f_change;
	else
	{
		std::cout << "\nERROR: " << FILELINE;
		m_evaluationFunction = &Condition::f_equals;//default to equals
		///ERROR LOG
	}
}


bool Condition::f_greaterThan(int value) const
{
	return (value > m_value);
}
bool Condition::f_lessThan(int value) const
{
	return (value < m_value);
}
bool Condition::f_equals(int value) const
{
	return (value == m_value);
}
bool Condition::f_notEquals(int value) const
{
	return (value != m_value);
}
bool Condition::f_change(int value) const
{
	(void)value;//shutup the compiler about unused
	return true;
}
