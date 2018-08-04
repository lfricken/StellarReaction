#ifndef GRAPPLEWEAPON_HPP
#define GRAPPLEWEAPON_HPP

#include "LaserWeapon.hpp"
#include "RayCastCallback.hpp"
#include "Beam.hpp"
#include "Timer.hpp"

struct GrappleWeaponData;

/// A weapon that pull other ships towards you.
class GrappleWeapon : public LaserWeapon
{
public:
	GrappleWeapon(const GrappleWeaponData& rData);
	virtual ~GrappleWeapon();
	///Actions to process on object after performing physics updates.
	void postPhysUpdate(const Vec2& center, const Vec2& aim, float32 radCCW, BodyComponent* pBody, float module_orientation);
protected:
	virtual Vec2 collisionHandle(const RayData& data);
private:
	void grappleTo();

	float m_pullStrength;
	Timer m_grappleTimer;
	FixtureComponent* m_target;
};


///Blueprint for GrappleWeapon.
struct GrappleWeaponData : public LaserWeaponData
{
	GrappleWeaponData() :
		LaserWeaponData(),
		pullStrength(100),
		pullTime(2)
	{
		weaponQuad.texName = "weapons/GrappleWeapon";
		damage = 0;
	}

	float pullStrength;
	float pullTime;

	///Create GrappleWeapon object from this data object.
	virtual Weapon* generate() const
	{
		return nullptr; // new GrappleWeapon(*this);
	}
	///Create new copy of this data object.
	virtual WeaponData* clone() const
	{
		return nullptr;// new GrappleWeaponData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};

#endif // GRAPPLEWEAPON_HPP
