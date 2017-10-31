#pragma once

#include "Projectile.hpp"

struct MissileData;
class Chunk;

/// <summary>
/// Projectile that follows target to deal damage.
/// </summary>
class Missile : public Projectile
{
public:
	Missile(const MissileData& rData);
	virtual ~Missile();

	/// <summary>
	/// Launches a missle with given direction, parent, target, position and acceleration.
	/// </summary>
	void missileLaunch(Vec2 rStart, wptr<Chunk>, float radCCW, float init_velocity, float acceleration, float max_velocity, int damage, const FixtureComponent* pParent, int collisions);


	void prePhysUpdate();
	void postPhysUpdate();

protected:
	Vec2 getTargetDirection(sptr<Chunk> target);
	void minimizeAngle(float& angle);
	void normalizeAngle(float& angle);
	wptr<Chunk> m_pTarget;
	float m_acceleration;
	float m_maxVelocity;

	void steer();
private:

};

/// Initialize Missile
struct MissileData : ProjectileData
{
	MissileData()
	{
		title = "missile_projectile";
		bodyComp.linearDampening = 1.7f;
		bodyComp.angularDampening = 6.7f;
	}


	///Create Missle object from this data object.
	virtual Missile* generate() const
	{
		MissileData copy(*this);
		return new Missile(copy);
	}
	///Create new copy of this data object.
	virtual MissileData* clone() const
	{
		return new MissileData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
	MyType(ProjectileData, MissileData);
};
