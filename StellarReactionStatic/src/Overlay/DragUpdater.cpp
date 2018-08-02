#include "DragUpdater.hpp"

#include "DragComponent.hpp"


using namespace leon;

DragUpdater::DragUpdater()
{

}
DragUpdater::~DragUpdater()
{

}
void DragUpdater::update(const sf::Vector2f& rPos)
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
		if((*it)->isDragging())
			(*it)->updateDrag(rPos);
}
void DragUpdater::give(DragComponent* pComp)
{
	m_components.push_back(pComp);
}
void DragUpdater::freeThis(DragComponent* pCompToDelete)
{
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
		if(*it == pCompToDelete)
		{
			m_components.erase(it);
			break;
		}
}
