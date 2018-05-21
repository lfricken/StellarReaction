#include "Camera.hpp"

#include "Globals.hpp"
#include "Convert.hpp"

const float Camera::m_maxZoom = 64;
const float Camera::m_minZoom = 1;
Camera::Camera()
{
	m_zoomLevel = 1;
	m_standardSize = static_cast<sf::Vector2f>(game.getWindow().getSize());
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
	m_view.setCenter(leon::b2Tosf<float>(shakePos));
}
void Camera::move(const Vec2& change)
{
	sf::Vector2f delta = leon::b2Tosf<float>(change);
	delta.x *= m_zoomLevel;
	delta.y *= m_zoomLevel;
	m_view.move(delta);

	Vec2 rPos = leon::sfTob2(m_view.getCenter());
	sf::Listener::setPosition(rPos.x, rPos.y, 5);///TODO: HOW DO WE SET Z OF LISTENER? is 5 far enough away? put this behind an api
}
void Camera::setZoom(float level)//multiple of each dimension to find new
{
	//level = 12;
	if(level>=m_minZoom && level<=m_maxZoom)
	{
		m_zoomLevel = level;
		m_view.setSize(m_standardSize.x*m_zoomLevel, m_standardSize.y*m_zoomLevel);
	}
}
void Camera::setRotation(float radiansCCW)
{
	m_view.setRotation(-leon::radToDeg(leon::normRad(radiansCCW)));
}
void Camera::resize()
{
	m_standardSize = static_cast<sf::Vector2f>(game.getWindow().getSize());
	m_view.setSize(m_standardSize.x * m_zoomLevel, m_standardSize.y *m_zoomLevel);
}


Vec2 Camera::getPosition() const
{
	return leon::sfTob2(m_view.getCenter());
}
float Camera::getZoom() const
{
	return m_zoomLevel;
}
float Camera::getRotation() const
{
	return -leon::degToRad(leon::normDeg(m_view.getRotation()));
}
const sf::View& Camera::getView() const
{
	return m_view;
}
