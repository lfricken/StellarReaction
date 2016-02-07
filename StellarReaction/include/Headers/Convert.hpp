#ifndef CONVERT_H
#define CONVERT_H

#include "stdafx.hpp"
#include "Globals.hpp"


/// <summary>
/// Useful conversions
/// </summary>
namespace leon
{

	/**=====RADIANS AND DEGREES=====**/
	template<typename T>
	inline float degToRad(T value)
	{
		return (pi*value)/180.0f;
	}
	template<typename T>
	inline float radToDeg(T value)
	{
		return (180.0f*value)/pi;
	}
	template<typename T>
	inline float Dist(const T& p1, const T& p2)
	{
		return sqrtf((p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y));
	}

	/**=====COORDINATE SYSTEMS=====**/
	template<typename T>
	inline T screenToCart(const T& screenCoords)//reverses y
	{
		return T(screenCoords.x, -screenCoords.y);
	}
	template<typename T>
	inline b2Vec2 sfTob2(const T& screenCoords)//used for converting from SFML to Box2D
	{
		return b2Vec2(screenCoords.x/scale, -screenCoords.y/scale);
	}
	template<typename T>
	inline sf::Vector2<T> b2Tosf(const b2Vec2& worldCoords)//World to sfml
	{
		return sf::Vector2<T>(worldCoords.x*scale, -worldCoords.y*scale);
	}
}

#endif // CONVERT_H
