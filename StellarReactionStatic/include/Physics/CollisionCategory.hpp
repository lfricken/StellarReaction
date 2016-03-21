#pragma once

#include "stdafx.hpp"

enum class Category
{
	None = 0x0000,

	Projectile = 0x0001,
	ShipModule = 0x0002,
	ShipForceField = 0x0004,
	Trigger = 0x0008,

	ShipModuleBroke = 0x0010,

	All = 0xFFFF,
};
Category ChooseCategory(const std::string& rChoice);

inline int operator |(int a, Category b)
{
	return static_cast<int>(a) | static_cast<int>(b);
}
inline int operator |(Category a, Category b)
{
	return static_cast<int>(static_cast<int>(a) | static_cast<int>(b));
}
inline int operator &(Category a, Category b)
{
	return static_cast<int>(static_cast<int>(a) & static_cast<int>(b));
}


enum class Mask
{
	None = static_cast<int>(Category::None),

	Projectile = static_cast<int>(Category::ShipModule) | static_cast<int>(Category::ShipForceField),
	ShipModule = static_cast<int>(Category::Projectile) | static_cast<int>(Category::Trigger) | static_cast<int>(Category::ShipModule) | static_cast<int>(Category::ShipModuleBroke),
	ShipForceField = static_cast<int>(Category::Projectile),
	Trigger = static_cast<int>(Category::All),

	Laser = static_cast<int>(Category::ShipModule),

	All = static_cast<int>(Category::All),
};
Mask ChooseMask(const std::string& rChoice);
