#include "GraphicsComponentUpdater.hpp"
#include "GraphicsComponent.hpp"

GraphicsComponentUpdater::GraphicsComponentUpdater()
{

}
GraphicsComponentUpdater::~GraphicsComponentUpdater()
{

}
void GraphicsComponentUpdater::store(GraphicsComponent* pTarget)
{
	m_targets.push_back(pTarget);
}
void GraphicsComponentUpdater::freeThis(GraphicsComponent* pTarget)
{
	auto it = m_targets.begin();

	for(; it!=m_targets.end(); ++it)
		if(pTarget == *it)
			break;

	m_targets.erase(it);
}
void GraphicsComponentUpdater::update()
{
	for(auto it = m_targets.begin(); it!=m_targets.end(); ++it)
		(*it)->update();
}
