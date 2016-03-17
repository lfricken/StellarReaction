#ifndef GRAPPLEWEAPON_HPP
#define GRAPPLEWEAPON_HPP

#include "LaserWeapon.hpp"
#include "RayCastCallback.hpp"
#include "Beam.hpp"

struct GrappleWeaponData;

class GrappleWeapon : public LaserWeapon
{
public:
	GrappleWeapon(const GrappleWeaponData& rData);
	virtual ~GrappleWeapon();

	void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation);
	void postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation);
protected:
private:
	void grappleTo(const b2Fixture& otherShip);
};



struct GrappleWeaponData : public LaserWeaponData
{
	GrappleWeaponData() :
		LaserWeaponData()
	{

	}

	virtual Weapon* generate() const
	{
		return new GrappleWeapon(*this);
	}
	virtual GrappleWeaponData* clone() const
	{
		return new GrappleWeaponData(*this);
	}

	virtual void loadJson(const Json::Value& root);

	MyType(WeaponData, GrappleWeaponData);
};

#endif // GRAPPLEWEAPON_HPP
