#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "stdafx.hpp"


/// The local players camera.
class Camera
{
public:
	Camera();
	virtual ~Camera();

	///Set the camera center to a world coordinate.
	void setPosition(const b2Vec2& rPos);
	///Move the camera center by a certain amount.
	void move(const b2Vec2& change);
	///Set the zoom level. Higher zoom makes the camera further away, so it can see more.
	void setZoom(float level);
	///Set the rotation of the camera in radians counter clockwise.
	void setRotation(float radiansCCW);
	///\brief If the windows resolution is resized, call this.
	///
	///It will resize the internal camera so the view is not skewed.
	void resize();

	///Return the current camera center.
	b2Vec2 getPosition() const;
	///Return the zoom value.
	float getZoom() const;
	///Return the rotation value.
	float getRotation() const;
	///Return the SFML view object.
	const sf::View& getView() const;

	/// Max zoom possible with a camera.
	const static float m_maxZoom;
	///Minimum zoom possible with a camera.
	const static float m_minZoom;
protected:
private:
	///Current zoom level between [1, m_minZoom]
	float m_zoomLevel;
	///Size of a zoom level 1 viewport.
	sf::Vector2f m_standardSize;
	///SFML view, which contains most internal workings of the Camera.
	sf::View m_view;
};

#endif // CAMERA_HPP
