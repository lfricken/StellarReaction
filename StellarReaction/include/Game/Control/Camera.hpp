#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "stdafx.hpp"

/// <summary>
/// The player camera, there is only one
/// </summary>
class Camera
{
public:
	Camera();
	virtual ~Camera();

	void setPosition(const b2Vec2& rPos);//world position
	void move(const b2Vec2& change);
	void setZoom(float level);//multiple of each dimension to find new
	void setRotation(float radiansCCW);

	b2Vec2 getPosition() const;
	float getZoom() const;
	float getRotation() const;
	const sf::View& getView() const;

protected:
private:
	const static float m_maxZoom;//max zoom possible with a camera
	const static float m_minZoom;//max zoom possible with a camera
	float m_zoomLevel;//current zoom between [1,16]
	sf::Vector2f m_standardSize;
	sf::View m_view;
};

#endif // CAMERA_HPP
