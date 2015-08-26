#include "Camera.hpp"

#include "Globals.hpp"
#include "Convert.hpp"

using namespace std;

const float Camera::m_maxZoom = 256;
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
void Camera::setPosition(const b2Vec2& rPos)//world position
{
	sf::Listener::setPosition(rPos.x, 0.f, 0.f);///HOW DO WE SET Z OF LISTENER rPos.y
	m_view.setCenter(leon::b2Tosf<float>(rPos));
}
void Camera::move(const b2Vec2& change)
{
	sf::Vector2f delta = leon::b2Tosf<float>(change);
	delta.x *= m_zoomLevel;
	delta.y *= m_zoomLevel;
	m_view.move(delta);

	b2Vec2 rPos = leon::sfTob2(m_view.getCenter());
	sf::Listener::setPosition(rPos.x, 0.f, 0.f);///HOW DO WE SET Z OF LISTENER rPos.y
}
void Camera::setZoom(float level)//multiple of each dimension to find new
{
	if(level>=m_minZoom && level<=m_maxZoom)
	{
		m_zoomLevel = level;
		m_view.setSize(m_standardSize.x*m_zoomLevel, m_standardSize.y*m_zoomLevel);
	}
}
void Camera::setRotation(float radiansCCW)
{
	m_view.setRotation(-leon::radToDeg(radiansCCW));
}



b2Vec2 Camera::getPosition() const
{
	return leon::sfTob2(m_view.getCenter());
}
float Camera::getZoom() const
{
	return m_zoomLevel;
}
float Camera::getRotation() const
{
	return -leon::degToRad(m_view.getRotation());
}
const sf::View& Camera::getView() const
{
	return m_view;
}
