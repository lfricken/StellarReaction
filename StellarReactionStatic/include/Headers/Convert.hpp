#pragma once

#include "stdafx.hpp"
#include "Globals.hpp"

/// <summary>
/// Useful conversions
/// </summary>
class Convert
{
public:
	template<typename T>
	/// <summary>
	/// Normalize radians to between 0 and 2PI.
	/// </summary>
	static inline float normRad(T value)
	{
		while(value >= Math::Tau)
			value -= Math::Tau;

		while(value < 0.f)
			value += Math::Tau;

		return value;
	}
	template<typename T>
	/// <summary>
	/// Normalize degrees to between 0 and 360.
	/// </summary>
	static inline float normDeg(T value)
	{
		while(value >= 360.f)
			value -= 360.f;

		while(value < 0.f)
			value += 360.f;

		return value;
	}
	template<typename T>
	/// <summary>
	/// Convert degrees to radians.
	/// </summary>
	static inline float degToRad(T value)
	{
		return (Math::Tau*value) / Math::Degrees;
	}
	template<typename T>
	/// <summary>
	/// Convert radians to degrees.
	/// </summary>
	static inline float radToDeg(T value)
	{
		return (Math::Degrees*value) / Math::Tau;
	}
	template<typename T>
	/// <summary>
	/// Distance formula for two 2D points.
	/// </summary>
	static inline float Dist(const T& p1, const T& p2)
	{
		return sqrtf((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
	}
	template<typename T>
	/// <summary>
	/// Negates y coordinate.
	/// </summary>
	static inline T flipYAxis(const T& screenCoords)//reverses y
	{
		return T(screenCoords.x, -screenCoords.y);
	}
	template<typename T>
	/// <summary>
	/// Take a dimension from SFML world coordinates (pixels) to universal coordinates.
	/// </summary>
	static inline T screenToUniverse(const T& value)
	{
		return value / static_cast<float>(standardModuleSize);
	}
	template<typename T>
	/// <summary>
	/// Take a dimension from universal to SFML world coordinates (pixels).
	/// </summary>
	static inline T universeToScreen(const T& value)
	{
		return value * static_cast<float>(standardModuleSize);
	}
	template<typename T>
	/// <summary>
	/// Convert from box2d to universal.
	/// </summary>
	static T worldToUniverse(const T& value)
	{
		return value / static_cast<float>(universeToWorldFactor);
	}
	template<typename T>
	/// <summary>
	/// Convert from universal to box2d.
	/// </summary>
	static T universeToWorld(const T& value)
	{
		return value * static_cast<float>(universeToWorldFactor);
	}

private:

	/// <summary>
	/// 1 Box2d unit in pixels at 1 zoom.
	/// </summary>
	static const int scale;
	/// <summary>
	/// size in pixels of module at 1 zoom
	/// </summary>
	static const int standardModuleSize;
	/// <summary>
	/// How big is 1 universal unit in b2Meter units.
	/// </summary>
	static const float universeToWorldFactor;
};