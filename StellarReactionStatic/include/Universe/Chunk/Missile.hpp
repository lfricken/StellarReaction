#pragma once

#include "Projectile.hpp"

struct MissileData;
class Chunk;

class Missile : public Projectile
{
public:
	Missile(const MissileData& rData);
	virtual ~Missile();

	void missileLaunch(b2Vec2 rStart, BodyComponent* target, float radCCW, float init_velocity, float acceleration, float max_velocity, int damage, const FixtureComponent* pParent, int collisions);
	void prePhysUpdate();
	void postPhysUpdate();
protected:
	b2Vec2 getTargetPos();
	void minimizeAngle(float& angle);
	void normalizeAngle(float& angle);
	BodyComponent* m_pTarget;
	float m_acceleration;
	float m_maxVelocity;
private:

};

struct MissileData : ProjectileData
{
	MissileData()
	{
		title = "missile_projectile";
		body.linearDampening = 1.7f;
		body.angularDampening = 6.7f;
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

	virtual void loadJson(const Json::Value& root);
	MyType(ProjectileData, MissileData);
};
