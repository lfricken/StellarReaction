#include "Beam.hpp"
#include "Convert.hpp"

Beam::Beam(const BeamData& rData) : QuadComponent(rData), m_startSprite(rData.start), m_endSprite(rData.end)
{
	m_startPos = (Vec2(0,0));
	m_endPos = (Vec2(0,0));

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
void Beam::setStart(const Vec2& rStart)
{
	m_startPos = rStart;
}
void Beam::setEnd(const Vec2& rEnd)
{
	m_endPos = rEnd;
}
void Beam::activate(float secs, int beamWidth, const sf::Color& color)//seconds to be active, and then deactivate, beam width pixels
{
	setColor(color);
	m_startSprite.setColor(color);
	m_endSprite.setColor(color);

	Vec2 midpoint = (m_startPos + m_endPos) / 2;
	float angle = atan2(m_startPos.y - m_endPos.y, m_startPos.x - m_endPos.x);

	m_deactivateTimer.setCountDown(secs);
	m_deactivateTimer.restartCountDown();

	m_startSprite.setPosition(m_startPos);
	m_startSprite.setRotation(angle);

	m_endSprite.setPosition(m_endPos);
	m_endSprite.setRotation(angle);

	setPosition(midpoint);
	setRotation(angle);

	float distance = Convert::universeToScreen(m_startPos.to(m_endPos).len());
	m_originPos[0] = sf::Vector2f(-distance/2.f, -beamWidth/2.f);
	m_originPos[1] = sf::Vector2f(-distance/2.f,  beamWidth/2.f);
	m_originPos[2] = sf::Vector2f( distance/2.f,  beamWidth/2.f);
	m_originPos[3] = sf::Vector2f( distance/2.f, -beamWidth/2.f);
}
void Beam::deactivate()
{
	sf::Color transparent = sf::Color(0,0,0,0);
	setColor(transparent);
	m_startSprite.setColor(transparent);
	m_endSprite.setColor(transparent);
}
void Beam::postUpdate()
{
	if(m_deactivateTimer.isTimeUp())
		deactivate();
}
