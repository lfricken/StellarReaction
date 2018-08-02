#include "Camera.hpp"

#include "Globals.hpp"
#include "Convert.hpp"

const float Camera::m_maxZoom = 64;
const float Camera::m_minZoom = 1;
Camera::Camera()
{
	m_zoomLevel = 1;
	m_standardSize = static_cast<sf::Vector2f>(getGame()->getWindow().getSize());
	m_view.setSize(m_standardSize);
	m_view.setCenter(0,0);
}
Camera::~Camera()
{

}
void Camera::shake(float duration, float frequency, float amplitude)
{
	m_shaker.generate(duration, frequency, amplitude);
}
void Camera::setPosition(const Vec2& rPos)//world position
{
	sf::Listener::setPosition(rPos.x, rPos.y, 5);///TODO: HOW DO WE SET Z OF LISTENER? is 5 far enough away? put this behind an api
	Vec2 shakePos = m_shaker.getPosition() + rPos;
	m_view.setCenter((sf::Vector2f)Convert::universeToScreen(Convert::flipYAxis(shakePos)));
}
void Camera::move(const Vec2& change)
{
	sf::Vector2f delta = ((sf::Vector2f)Convert::universeToScreen(Convert::flipYAxis(change)));
	delta.x *= m_zoomLevel;
	delta.y *= m_zoomLevel;
	m_view.move(delta);

	sf::Vector2f rPos = m_view.getCenter(); // TODO leon::sfTob2( used to be here???
	sf::Listener::setPosition(rPos.x, rPos.y, 5);///TODO: HOW DO WE SET Z OF LISTENER? is 5 far enough away? put this behind an api
}
void Camera::setZoom(float level)//multiple of each dimension to find new
{
	m_zoomLevel = level;
	m_view.setSize(m_standardSize.x*m_zoomLevel, m_standardSize.y*m_zoomLevel);

	if(m_zoomLevel < m_minZoom)
	{
		setZoom(m_minZoom);
	}
	else if(m_zoomLevel > m_maxZoom)
	{
		setZoom(m_maxZoom);
	}
}
void Camera::setRotation(float radiansCCW)
{
	m_view.setRotation(-Convert::radToDeg(Convert::normRad(radiansCCW)));
}
void Camera::resize()
{
	m_standardSize = static_cast<sf::Vector2f>(getGame()->getWindow().getSize());
	m_view.setSize(m_standardSize.x * m_zoomLevel, m_standardSize.y *m_zoomLevel);
}


Vec2 Camera::getPosition() const
{
	return (Vec2)Convert::flipYAxis(Convert::screenToUniverse(m_view.getCenter()));
}
float Camera::getZoom() const
{
	return m_zoomLevel;
}
float Camera::getRotation() const
{
	return -Convert::degToRad(Convert::normDeg(m_view.getRotation()));
}
const sf::View& Camera::getView() const
{
	return m_view;
}
