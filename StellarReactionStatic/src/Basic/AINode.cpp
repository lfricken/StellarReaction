#include "AINode.hpp"

AINode::AINode()
{

}
AINode::~AINode()
{

}
const Vec2& AINode::getPosition() const
{
	return m_position;
}
void AINode::setPosition(const Vec2& newPos)
{
	m_position = newPos;
}
