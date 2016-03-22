#ifndef BALLISTICWEAPON_HPP
#define BALLISTICWEAPON_HPP

#include "Weapon.hpp"
struct BallisticWeaponData;

/// <summary>
/// Fires a physical projectile to do something (like damage)
/// </summary>
class BallisticWeapon : public Weapon
{
public:
	BallisticWeapon(const BallisticWeaponData& rData);
	virtual ~BallisticWeapon();

	void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation);
	void postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation);
protected:

	float m_projLifetime;
	std::string m_projName;//bp name of projectile
	float m_velocity;//velocity of projectiles
private:
};



struct BallisticWeaponData : public WeaponData
{
	BallisticWeaponData() :
		velocity(50),
		projName("DefaultProjectile")
	{
		weaponQuad.animSheetName = "weapons/ballistic1.acfg";
		weaponQuad.texName = "weapons/missile1.png";
		energyConsumption = 1;///TODO SHOULD BE CONSUMING BALLISTIC ONLY, THIS IS JUST FOR TESTING
		ballisticConsumption = 0;
		missileConsumption = 0;
		damage = 500;
		shotDelay = 0.3f;
		shots = 1;
		fireDelay = 1;
		range = 45.0f;
		radCCWps = 0;
	}

	std::string projName;//the bp name of the projectile we fire, to be passed to projectileMan
	float velocity;//m/s (blocks per second)
	float radCCWps;//rotation rate of the projectile Radians CCW per second

	virtual Weapon* generate() const
	{
		return new BallisticWeapon(*this);
	}
	virtual BallisticWeaponData* clone() const
	{
		return new BallisticWeaponData(*this);
	}

	virtual void loadJson(const Json::Value& root);

	MyType(WeaponData, BallisticWeaponData);
};

#endif // BALLISTICWEAPON_HPP
