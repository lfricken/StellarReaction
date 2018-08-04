#pragma once

#include "ProjectileWeaponBase.hpp"
struct MissileWeaponBlueprint;

///Fires a Missile to deal damage to a target. Will target nearest Ship to mouse cursor.
class MissileWeapon : public ProjectileWeaponBase
{
public:
	virtual ~MissileWeapon();
	///Actions to be processed before a shot is taken.
	virtual void preShot(const ShotData& data);
	///Actions to be processed after a shot is taken.
	virtual void postShot(const ShotData& data);


	struct Data
	{
		/// <summary>
		/// True if when launching the missile nose should point at the target.
		/// </summary>
		bool alignWithAim;

		/// <summary>
		/// If we don't align with aim, where should our nose should point when we launch in relation to the ship?
		/// </summary>
		float alignAngle;

		/// <summary>
		/// True if the missile initial velocity should point at the target.
		/// </summary>
		bool launchVelocityWithAim;

		/// <summary>
		/// If we don't launch with aim, where should our initial velocity point in relation to the ship?
		/// </summary>
		float launchVelocityAngle;

		/// <summary>
		/// How much does this missile accelerate.
		/// </summary>
		float acceleration;

		/// <summary>
		/// Missile will not accelerate past this speed.
		/// </summary>
		float maxVelocity;
	};

protected:
	MissileWeapon(const MissileWeaponBlueprint& rData);

	Data missileData;

	friend struct MissileWeaponBlueprint;
};


/// Blueprint for MissileWeapon.
struct MissileWeaponBlueprint : public ProjectileWeaponBaseData
{
	MissileWeaponBlueprint()
	{
		weaponQuad.texName = "weapons/MissileWeapon";
		energyConsumption = 1;///TODO SHOULD BE CONSUMING BALLISTIC ONLY, THIS IS JUST FOR TESTING
		ballisticConsumption = 0;
		missileConsumption = 0;
		damage = 1;
		shotDelay = 1.f;
		shots = 1;
		fireDelay = 1.f;
		range = 45.0f;
	}
	
	MissileWeapon::Data missileData;

	///Create MissileWeapon object from this data object.
	virtual Weapon* generate() const
	{
		return new MissileWeapon(*this);
	}
	///Create new copy of this data object.
	virtual WeaponData* clone() const
	{
		return new MissileWeaponBlueprint(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};
