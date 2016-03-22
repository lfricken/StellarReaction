#pragma once

#include "Weapon.hpp"
struct MissileWeaponData;

/// <summary>
/// Fires a missile that tracks its target
/// </summary>
class MissileWeapon : public Weapon
{
public:
	MissileWeapon(const MissileWeaponData& rData);
	virtual ~MissileWeapon();

	void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation);
	void postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation);
protected:
	float m_projLifetime;
	std::string m_projName;//bp name of projectile
	float m_velocity;//velocity of projectiles
	float m_acceleration;
	int m_max_velocity;
	int m_init_velocity;
private:
};



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

	virtual Weapon* generate() const
	{
		return new MissileWeapon(*this);
	}
	virtual MissileWeaponData* clone() const
	{
		return new MissileWeaponData(*this);
	}

	virtual void loadJson(const Json::Value& root);

	MyType(WeaponData, MissileWeaponData);
};
