#pragma once

#include "stdafx.hpp"

/// <summary>
/// What we are.
/// </summary>
enum class Category
{
	None = 0x0000,

	Projectile = 0x0001,
	ShipModule = 0x0002,
	ShipForceField = 0x0004,
	Trigger = 0x0008,

	All = 0xFFFF,
};
///Converts Category String into enum class.
Category ChooseCategory(const String& rChoice);
///Bitwise OR between integer and category.
inline int operator |(int a, Category b)
{
	return static_cast<int>(a) | static_cast<int>(b);
}
///Bitwise OR between category and category.
inline int operator |(Category a, Category b)
{
	return static_cast<int>(static_cast<int>(a) | static_cast<int>(b));
}
///Bitwise AND between category and category.
inline int operator &(Category a, Category b)
{
	return static_cast<int>(static_cast<int>(a) & static_cast<int>(b));
}

/// <summary>
/// What we collide with, setup as a (what we are)
/// so for instance, ShipModule only tries to collide with itself, triggers, and projectiles
/// </summary>
enum class Mask
{
	None = static_cast<int>(Category::None),

	Projectile = static_cast<int>(Category::ShipModule) | static_cast<int>(Category::ShipForceField),
	ShipModule = static_cast<int>(Category::Projectile) | static_cast<int>(Category::Trigger) | static_cast<int>(Category::ShipModule),
	ShipForceField = static_cast<int>(Category::Projectile),
	Trigger = static_cast<int>(Category::ShipModule),

	Laser = static_cast<int>(Category::ShipModule) | static_cast<int>(Category::ShipForceField),

	All = static_cast<int>(Category::All),
};
///Converts Mask String into enum class.
Mask ChooseMask(const String& rChoice);
