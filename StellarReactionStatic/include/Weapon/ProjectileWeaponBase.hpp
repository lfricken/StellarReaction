#pragma once

#include "Weapon.hpp"
#include "Projectile.hpp"

struct ProjectileWeaponBaseData;

class ProjectileWeaponBase : public Weapon
{
public:
	~ProjectileWeaponBase();

	struct Data
	{
		/// <summary>
		/// How long will this projectile last before exploding.
		/// </summary>
		float lifetime;

		/// <summary>
		/// Blueprint to use for the projectile.
		/// </summary>
		String blueprint;

		/// <summary>
		/// Initial velocity of the projectile.
		/// </summary>
		float velocity;
	};

protected:
	ProjectileWeaponBase(const ProjectileWeaponBaseData& data);

	Projectile::LaunchData generateLaunchData(const Vec2& startPosition, const Vec2& targetPosition);

	Data projectileData;
};

struct ProjectileWeaponBaseData : public WeaponData
{

	ProjectileWeaponBase::Data projectileData;

	virtual Weapon* generate() const
	{
		WARNING;
		return nullptr;
	}

	virtual WeaponData* clone() const
	{
		WARNING;
		return nullptr;
	}

	virtual void loadJson(const Json::Value& root);
};
