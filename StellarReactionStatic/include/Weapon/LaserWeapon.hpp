#ifndef LASERWEAPON_HPP
#define LASERWEAPON_HPP

#include "Weapon.hpp"
#include "RayCastCallback.hpp"
#include "Beam.hpp"

struct LaserWeaponData;

class LaserWeapon : public Weapon
{
public:
	LaserWeapon(const LaserWeaponData& rData);
	virtual ~LaserWeapon();

	void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation);
	void postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation);
protected:
	virtual Vec2 collisionHandle(const RayData& data);

	RayCastCallback m_ray;
	int m_beamWidth;
	sf::Color m_beamColor;
	Beam m_beam;
	float m_showTime;
private:

};



struct LaserWeaponData : public WeaponData
{
	LaserWeaponData() :
		beamWidth(32),
		beamColor(sf::Color::White),
		showTime(0.03f)
	{
		energyConsumption = 1;///TODO SHOULD BE CONSUMING BALLISTIC ONLY, THIS IS JUST FOR TESTING
		ballisticConsumption = 0;
		missileConsumption = 0;
		damage = 1;
		shotDelay = 1.f;
		shots = 1;
		fireDelay = 1;
		range = 45.0f;
		weaponQuad.animSheetName = "weapons/LaserWeapon.acfg";
		weaponQuad.texName = "weapons/LaserWeapon.png";
	}

	int beamWidth;
	sf::Color beamColor;
	float showTime;//how long to display the beam per shot
	BeamData beamComp;

	virtual Weapon* generate() const
	{
		return new LaserWeapon(*this);
	}
	virtual LaserWeaponData* clone() const
	{
		return new LaserWeaponData(*this);
	}

	virtual void loadJson(const Json::Value& root);

	MyType(WeaponData, LaserWeaponData);
};

#endif // LASERWEAPON_HPP
