#ifndef LASERWEAPON_HPP
#define LASERWEAPON_HPP

#include "Weapon.hpp"
#include "RayCastCallback.hpp"
#include "Beam.hpp"

struct LaserWeaponData;

/// A weapon that fires bursts of lasers to do damage.
class LaserWeapon : public Weapon
{
public:
	LaserWeapon(const LaserWeaponData& rData);
	virtual ~LaserWeapon();
	///Actions to be processed before a shot is taken.
	void preShot(const ShotData& data);
	///Actions to be processed after a shot is taken.
	void postShot(const ShotData& data);
protected:

	RayCastCallback m_ray;
	int m_beamWidth;
	sf::Color m_beamColor;
	Beam m_beam;
	float m_showTime;
private:

};

///Blueprint for LaserWeapon.
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
		weaponQuad.texName = "weapons/LaserWeapon";
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
	virtual WeaponData* clone() const
	{
		return new LaserWeaponData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};

#endif // LASERWEAPON_HPP
