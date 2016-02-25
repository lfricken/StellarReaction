#ifndef COLLISIONCATEGORY_H
#define COLLISIONCATEGORY_H

#include "stdafx.hpp"


/// <summary>
/// asdf
/// </summary>
enum class Category
{
	None = 0x0000,//we aren't anything! 0000 0000 0000 0000

	Projectile = 0x0001,              //0000 0000 0000 0001
	ShipModule = 0x0002,             // 0000 0000 0000 0010
	ShipModuleBroke = 0x0004,       //  0000 0000 0000 0100
	ShipForceField = 0x0008,        //  0000 0000 0000 1000 //wait, if we don't have this, we wont collide with sensors??? that could be a problem (WHAT?)

	///ShipHullSensor = 0x0010,
	///ShipHull = 0x0010,
	Actor = 0x0020,
	Item = 0x0040,
	Trigger = 0x0080,

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

/**What things should we collide with?**/
enum class Mask
{
	None = static_cast<int>(Category::None),

	Projectile = static_cast<int>(Category::ShipModule) | static_cast<int>(Category::ShipForceField),
	Laser = static_cast<int>(Category::ShipModule),

	ShipModule = static_cast<int>(Category::Projectile) | static_cast<int>(Category::Trigger) | static_cast<int>(Category::ShipModule) | static_cast<int>(Category::ShipModuleBroke),

	ShipForceField = static_cast<int>(Category::Projectile),

	Trigger = static_cast<int>(Category::All),

	All = static_cast<int>(Category::All),
};
Mask ChooseMask(const std::string& rChoice);



#endif // COLLISIONCATEGORY_H
