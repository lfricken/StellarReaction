#ifndef GRAPPLEWEAPON_HPP
#define GRAPPLEWEAPON_HPP

#include "LaserWeapon.hpp"
#include "RayCastCallback.hpp"
#include "Beam.hpp"
#include "Timer.hpp"

struct GrappleWeaponData;

class GrappleWeapon : public LaserWeapon
{
public:
	GrappleWeapon(const GrappleWeaponData& rData);
	virtual ~GrappleWeapon();

	void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation);
	void postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation);
	void postPhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 radCCW, b2Body* pBody, float module_orientation);
protected:
private:
	void grappleTo();
	Timer m_time;
	FixtureComponent* target;
};



struct GrappleWeaponData : public LaserWeaponData
{
	GrappleWeaponData() :
		LaserWeaponData()
	{
		weaponQuad.animSheetName = "weapons/GrappleWeapon.acfg";
		weaponQuad.texName = "weapons/GrappleWeapon.png";
		damage = 0;
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
