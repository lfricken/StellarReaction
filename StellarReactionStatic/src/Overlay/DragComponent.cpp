#include "DragComponent.hpp"

#include "DragUpdater.hpp"

using namespace leon;

DragComponent::~DragComponent()
{
	m_pOwner->freeThis(this);
}
bool DragComponent::isDragging() const
{
	return m_isDragging;
}
void DragComponent::toggleDragging(bool dragging)
{
	m_isDragging = dragging;
}
void DragComponent::updateDrag(const sf::Vector2f& rPos)
{
	m_cbFunction(rPos);
}
void DragComponent::f_giveThyself()
{
	m_pOwner->give(this);
}