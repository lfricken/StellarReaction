#pragma once

#include "stdafx.hpp"
#include "Globals.hpp"



/// <summary>
/// Useful conversions
/// </summary>
namespace leon
{
	/** Normalize Radians and Degrees **/
	template<typename T>
	/// Normalize radians to between 0 and 2PI.
	inline float normRad(T value)
	{
		float pi2 = 2.f * pi;
		while (value > pi2)
			value -= pi2;

		while (value < 0.f)
			value += pi2;

		return value;
	}
	template<typename T>
	inline float normDeg(T value)
	{
		while (value > 360.f)
			value -= 360.f;

		while (value < 0.f)
			value += 360.f;

		return value;
	}

	/**=====RADIANS AND DEGREES=====**/
	template<typename T>
	inline float degToRad(T value)
	{
		return (pi*value) / 180.0f;
	}
	template<typename T>
	inline float radToDeg(T value)
	{
		return (180.0f*value) / pi;
	}
	



	template<typename T>
	/// Distance formula for two 2D points.
	inline float Dist(const T& p1, const T& p2)
	{
		return sqrtf((p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y));
	}
	template<typename T>
	/// Negates y coordinate.
	inline T screenToCart(const T& screenCoords)//reverses y
	{
		return T(screenCoords.x, -screenCoords.y);
	}
	template<typename T>
	/// Take a coordinate from SFML world coordinates (pixels) to Box2D world coordinates (meters).
	inline b2Vec2 sfTob2(const T& screenCoords)
	{
		return b2Vec2(screenCoords.x/scale, -screenCoords.y/scale);
	}
	template<typename T>
	/// Take a coordinate from Box2D world coordinates (meters) to SFML world coordinates (pixels).
	inline sf::Vector2<T> b2Tosf(const b2Vec2& worldCoords)
	{
		return sf::Vector2<T>(worldCoords.x*scale, -worldCoords.y*scale);
	}
}