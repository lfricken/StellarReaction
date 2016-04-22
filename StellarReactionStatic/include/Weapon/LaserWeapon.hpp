#ifndef LASERWEAPON_HPP
#define LASERWEAPON_HPP

#include "Weapon.hpp"
#include "RayCastCallback.hpp"
#include "Beam.hpp"

struct LaserWeaponData;

/* 
* LaserWeapon Class:
* Extends Weapon to implement laser weapon game objects.
* Laser Weapon game objects fire energy projectiles that do something.
*/

class LaserWeapon : public Weapon
{
public:
	LaserWeapon(const LaserWeaponData& rData);
	virtual ~LaserWeapon();
	///Actions to be processed before a shot is taken.
	void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation);
	///Actions to be processed after a shot is taken.
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
	//how long to display the beam per shot
	float showTime;
	BeamData beamComp;
	///Create LaserWeapon object from this data object.
	virtual Weapon* generate() const
	{
		return new LaserWeapon(*this);
	}
	///Create new copy of this data object.
	virtual LaserWeaponData* clone() const
	{
		return new LaserWeaponData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(WeaponData, LaserWeaponData);
};

#endif // LASERWEAPON_HPP
