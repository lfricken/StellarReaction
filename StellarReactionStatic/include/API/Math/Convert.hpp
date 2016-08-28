#pragma once

#include "Constant.hpp"
namespace Math
{
	/// <summary>
	/// Convert from degrees to radians.
	/// </summary>
	template<typename T>
	T toRad(T value)
	{
		return (Constant::Tau*value) / Constant::Degrees;
	}

	/// <summary>
	/// Convert from radians to degrees.
	/// </summary>
	template<typename T>
	T toDeg(T value)
	{
		return (Constant::Degrees*value) / Constant::Tau;
	}

	/// <summary>
	/// Normalize radians between 0 and Tau.
	/// </summary>
	template<typename T>
	T normalizeRad(T value)
	{
		while(value > Constant::Tau)
			value -= Constant::Tau;

		while(value < 0.f)
			value += Constant::Tau;

		return value;
	}
}

