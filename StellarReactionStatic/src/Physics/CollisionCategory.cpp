#include "CollisionCategory.hpp"

Category ChooseCategory(const std::string& rChoice)
{
	if(rChoice == "None")
		return Category::None;
	else if(rChoice == "Projectile")
		return Category::Projectile;
	else if(rChoice == "ShipModule")
		return Category::ShipModule;
	else if(rChoice == "ShipModuleBroke")
		return Category::ShipModuleBroke;
	else if(rChoice == "ShipForceField")
		return Category::ShipForceField;
	else if(rChoice == "Actor")
		return Category::Actor;
	else if(rChoice == "Item")
		return Category::Item;
	else if(rChoice == "Trigger")
		return Category::Trigger;
	else if(rChoice == "All")
		return Category::All;
	else
	{
		///Error: debug
		return Category::None;
	}
}
Mask ChooseMask(const std::string& rChoice)
{
	if(rChoice == "None")
		return Mask::None;
	else if(rChoice == "Projectile")
		return Mask::Projectile;
	else if(rChoice == "Laser")
		return Mask::Laser;
	else if(rChoice == "ShipModule")
		return Mask::ShipModule;
	else if(rChoice == "ShipForceField")
		return Mask::ShipForceField;
	else if(rChoice == "Trigger")
		return Mask::Trigger;
	else if(rChoice == "All")
		return Mask::All;
	else
	{
		///Error: debug
		return Mask::None;
	}
}
