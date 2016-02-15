#pragma once

#include "Projectile.hpp"

struct MissileData;

class Missile : public Projectile
{
public:
	Missile(const MissileData& rData);
	virtual ~Missile();

	void missileLaunch();

protected:

private:

};

struct MissileData : ProjectileData
{
	MissileData()
	{
		title = "missile_projectile";
	}

	virtual Missile* generate() const
	{
		MissileData copy(*this);
		return new Missile(copy);
	}
	virtual MissileData* clone() const
	{
		return new MissileData(*this);
	}
};
