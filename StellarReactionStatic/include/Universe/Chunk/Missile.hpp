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
	/// Launches a missile with given stats.
	/// </summary>
	static void launchNew(ProjectileMan* manager, LaunchData data);

	void prePhysUpdate();
	void postPhysUpdate();

protected:

	virtual void launch(const LaunchData& data);

	Vec2 getTargetDirection(Chunk* target);
	void minimizeAngle(float& angle);
	void normalizeAngle(float& angle);
	Chunk* m_pTarget;
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

	virtual Projectile* generate() const
	{
		MissileData copy(*this);
		return new Missile(copy);
	}

	virtual ProjectileData* clone() const
	{
		return new MissileData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};
