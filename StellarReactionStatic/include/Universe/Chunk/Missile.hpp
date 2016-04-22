#pragma once

#include "Projectile.hpp"

struct MissileData;
class Chunk;


/*
* Missle Class:
* Extends Projectile to create missle game object.
*/
class Missile : public Projectile
{
public:
	Missile(const MissileData& rData);
	virtual ~Missile();
	///Launches a missle with given direction, parent, target, position and acceleration.
	void missileLaunch(b2Vec2 rStart, BodyComponent* target, float radCCW, float init_velocity, float acceleration, float max_velocity, int damage, const FixtureComponent* pParent, int collisions);
	///Actions to process on object before performing physics updates.
	void prePhysUpdate();
	///Actions to process on chunk after performing physics updates.
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
