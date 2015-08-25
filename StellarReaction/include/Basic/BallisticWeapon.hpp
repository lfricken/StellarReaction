#ifndef BALLISTICWEAPON_HPP
#define BALLISTICWEAPON_HPP

#include "Weapon.hpp"
struct BallisticWeaponData;

class BallisticWeapon : public Weapon
{
public:
	BallisticWeapon(const BallisticWeaponData& rData);
	virtual ~BallisticWeapon();

	void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW);
	void postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW);
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
		weaponQuad.texName = "weapons/ballistic1.png";
		ener = 1;///SHOULD BE CONSUMING BALLISTIC, THIS IS JUST FOR TESTING
		ball = 0;
		mis = 0;
		damage = 500;
		shotDelay = 0.3f;
		shots = 1;
		fireDelay = 1;
		range = 45.0f;
		radCCWps = 0;
	}

	std::string projName;//the bp name of the projectile we fire, to be passed to projectileMan
	float velocity;//m/s (blocks per second)
	float radCCWps;//rotation rate Radians CCW per second

	virtual Weapon* generate() const
	{
		return new BallisticWeapon(*this);
	}
	virtual BallisticWeaponData* clone() const
	{
		return new BallisticWeaponData(*this);
	}
};

#endif // BALLISTICWEAPON_HPP
