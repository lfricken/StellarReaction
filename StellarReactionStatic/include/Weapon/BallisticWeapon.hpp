#ifndef BALLISTICWEAPON_HPP
#define BALLISTICWEAPON_HPP

#include "ProjectileWeaponBase.hpp"
struct BallisticWeaponData;

/// Fire a Projectile at a target to do damage.
class BallisticWeapon : public ProjectileWeaponBase
{
public:
	BallisticWeapon(const BallisticWeaponData& rData);
	virtual ~BallisticWeapon();
	///Actions to be processed before a shot is taken.
	virtual void preShot(const ShotData& data);
	///Actions to be processed after a shot is taken.
	virtual void postShot(const ShotData& data);
protected:

private:
};


/// Blueprint for BallisticWeapon.
struct BallisticWeaponData : public ProjectileWeaponBaseData
{
	BallisticWeaponData()
	{
		weaponQuad.texName = "weapons/BallisticWeapon";
		energyConsumption = 1;///TODO SHOULD BE CONSUMING BALLISTIC ONLY, THIS IS JUST FOR TESTING
		ballisticConsumption = 0;
		missileConsumption = 0;
		damage = 1;
		shotDelay = 1.f;
		shots = 1;
		fireDelay = 1;
		range = 45.0f;
	}

	///Create BallisticWeapon object from this data object.
	virtual Weapon* generate() const
	{
		return new BallisticWeapon(*this);
	}
	///Create new copy of this data object.
	virtual WeaponData* clone() const
	{
		return new BallisticWeaponData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};

#endif // BALLISTICWEAPON_HPP
