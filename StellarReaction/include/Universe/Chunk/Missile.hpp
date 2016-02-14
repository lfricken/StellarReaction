#pragma once

#include "Projectile.hpp"

struct MissileData;
class Chunk;

class Missile : public Projectile
{
public:
	Missile(const MissileData& rData);
	virtual ~Missile();

	void missileLaunch(b2Vec2 rStart, Chunk* target, float radCCW, int init_velocity, int acceleration, int max_velocity, int damage, const FixtureComponent* pParent, int collisions);
	void prePhysUpdate();
	void postPhysUpdate();
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
