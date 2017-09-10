#include "Condition.hpp"
#include "stdafx.hpp"
#include "Globals.hpp"
#include "Debugging.hpp"

Condition::Condition()
{
	reset(EventType::None, 0, '=', false);//default
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
	m_op = comparison;
	m_isRepeatable = repeatable;
}
EventType Condition::getEventType() const
{
	return m_eventType;
}
char Condition::getComparison() const
{
	return m_op;
}
bool Condition::isRepeatable() const
{
	return m_isRepeatable;
}
bool Condition::evaluate(int value) const
{
	if(m_op == '>')
		return value > m_value;
	if(m_op == '<')
		return value < m_value;
	if(m_op == '=')
		return value == m_value;
	if(m_op == '!')
		return value != m_value;
	if(m_op == 'd')
		return true;

	WARNING;

	return false;
}







