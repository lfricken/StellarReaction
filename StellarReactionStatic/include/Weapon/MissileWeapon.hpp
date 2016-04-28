#pragma once

#include "Weapon.hpp"
struct MissileWeaponData;

///Fires a Missile to deal damage to a target. Will target nearest Ship to mouse cursor.
class MissileWeapon : public Weapon
{
public:
	MissileWeapon(const MissileWeaponData& rData);
	virtual ~MissileWeapon();
	///Actions to be processed before a shot is taken.
	void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation);
	///Actions to be processed after a shot is taken.
	void postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation);
protected:
	float m_projLifetime;
	///bp name of projectile
	std::string m_projName;
	///velocity of projectiles
	float m_velocity;
	float m_acceleration;
	float m_max_velocity;
	float m_init_velocity;
private:
};


/// Blueprint for MissileWeapon.
struct MissileWeaponData : public WeaponData
{
	MissileWeaponData() :
		velocity(20),
		projName("DefaultMissile")
	{
		weaponQuad.animSheetName = "weapons/MissileWeapon.acfg";
		weaponQuad.texName = "weapons/MissileWeapon.png";
		energyConsumption = 1;///TODO SHOULD BE CONSUMING BALLISTIC ONLY, THIS IS JUST FOR TESTING
		ballisticConsumption = 0;
		missileConsumption = 0;
		damage = 1;
		shotDelay = 1.f;
		shots = 1;
		fireDelay = 1.f;
		range = 45.0f;
		radCCWps = 0;
	}

	std::string projName;//the bp name of the projectile we fire, to be passed to projectileMan
	float velocity;//m/s (blocks per second)
	float radCCWps;//rotation rate of the projectile Radians CCW per second

	///Create MissileWeapon object from this data object.
	virtual Weapon* generate() const
	{
		return new MissileWeapon(*this);
	}
	///Create new copy of this data object.
	virtual MissileWeaponData* clone() const
	{
		return new MissileWeaponData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(WeaponData, MissileWeaponData);
};
