#ifndef BALLISTICWEAPON_HPP
#define BALLISTICWEAPON_HPP

#include "Weapon.hpp"
struct BallisticWeaponData;

/// Fire a Projectile at a target to do damage.
class BallisticWeapon : public Weapon
{
public:
	BallisticWeapon(const BallisticWeaponData& rData);
	virtual ~BallisticWeapon();
	///Actions to be processed before a shot is taken.
	void preShot(const Vec2& center, const Vec2& aim, float radCCW, float module_orientation);
	///Actions to be processed after a shot is taken.
	void postShot(const Vec2& center, const Vec2& aim, float radCCW, float module_orientation);
protected:

	float m_projLifetime;
	///Blueprint name of the projetile
	String m_projName;
	///Velocity of the projectile
	float m_velocity;
private:
};


/// Blueprint for BallisticWeapon.
struct BallisticWeaponData : public WeaponData
{
	BallisticWeaponData() :
		velocity(50),
		projName("DefaultProjectile")
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
		radCCWps = 0;
	}

	///the Blueprint name of the projectile we fire, to be passed to projectileMan
	String projName;
	///m/s (blocks per second)
	float velocity;
	///rotation rate of the projectile Radians CCW per second
	float radCCWps;

	///Create BallisticWeapon object from this data object.
	virtual Weapon* generate() const
	{
		return new BallisticWeapon(*this);
	}
	///Create new copy of this data object.
	virtual BallisticWeaponData* clone() const
	{
		return new BallisticWeaponData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(WeaponData, BallisticWeaponData);
};

#endif // BALLISTICWEAPON_HPP
