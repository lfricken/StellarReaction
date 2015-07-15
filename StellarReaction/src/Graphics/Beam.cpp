#include "Beam.hpp"

#include "Globals.hpp"

using namespace std;

Beam::Beam(const BeamData& rData) : QuadComponent(rData), m_start(rData.start), m_end(rData.end)
{
	m_startPos = (b2Vec2(0,0));
	m_endPos = (b2Vec2(0,0));

	m_deactivateTimer.setCountDown(0);

	m_originPos.resize(m_numVerts);
	m_originPos[0] = sf::Vector2f(-rData.dimensions.x/2, -rData.dimensions.y/2);
	m_originPos[1] = sf::Vector2f(-rData.dimensions.x/2,  rData.dimensions.y/2);
	m_originPos[2] = sf::Vector2f( rData.dimensions.x/2,  rData.dimensions.y/2);
	m_originPos[3] = sf::Vector2f( rData.dimensions.x/2, -rData.dimensions.y/2);
}
Beam::~Beam()
{

}
void Beam::setStart(const b2Vec2& rStart)
{
	m_startPos = rStart;
}
void Beam::setEnd(const b2Vec2& rEnd)
{
	m_endPos = rEnd;
}
void Beam::activate(float secs, int beamWidth, const sf::Color& color)//seconds to be active, and then deactivate, beam width pixels
{
	setColor(color);
	m_start.setColor(color);
	m_end.setColor(color);

	b2Vec2 midpoint;
	midpoint.x = (m_startPos.x+m_endPos.x)/2;
	midpoint.y = (m_startPos.y+m_endPos.y)/2;
	float angle = atan2(m_startPos.y - m_endPos.y, m_startPos.x - m_endPos.x);
	float distance = scale*sqrtf((m_endPos.x-m_startPos.x)*(m_endPos.x-m_startPos.x)  +  (m_endPos.y-m_startPos.y)*(m_endPos.y-m_startPos.y));

	m_deactivateTimer.setCountDown(secs);
	m_deactivateTimer.restartCountDown();

	m_start.setPosition(m_startPos);
	m_start.setRotation(angle);

	m_end.setPosition(m_endPos);
	m_end.setRotation(angle);

	setPosition(midpoint);
	setRotation(angle);

	m_originPos[0] = sf::Vector2f(-distance/2.f, -beamWidth/2.f);
	m_originPos[1] = sf::Vector2f(-distance/2.f,  beamWidth/2.f);
	m_originPos[2] = sf::Vector2f( distance/2.f,  beamWidth/2.f);
	m_originPos[3] = sf::Vector2f( distance/2.f, -beamWidth/2.f);
}
void Beam::deactivate()
{
	sf::Color transparent = sf::Color(0,0,0,0);
	setColor(transparent);
	m_start.setColor(transparent);
	m_end.setColor(transparent);
}
void Beam::postUpdate()
{
	if(m_deactivateTimer.isTimeUp())
		deactivate();
}
