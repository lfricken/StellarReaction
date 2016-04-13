#include "LinearMeter.hpp"

LinearMeter::LinearMeter(const LinearMeterData& rData) : QuadComponent(rData)
{
	m_percent = 0;
}
LinearMeter::~LinearMeter()
{

}
void LinearMeter::postUpdate()//used to override texCoords
{
	sf::Vector2i tile = m_animator.getTile();
	sf::Vector2f size = m_animator.getTileSize();

	for(int i=0; i<m_numVerts; ++i)
		(*m_pVerts)[i+m_startVert].texCoords = sf::Vector2f(size.x*m_originTex[i].x+tile.x*size.x, size.y*m_originTex[i].y+m_percent*size.y);
}
void LinearMeter::setPercent(float per)// 0-1
{
	m_percent = per;
}
