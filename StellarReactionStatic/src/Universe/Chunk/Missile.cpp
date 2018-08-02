#include "Missile.hpp"
#include "Convert.hpp"
#include "Chunk.hpp"




Missile::Missile(const MissileData& rData) : Projectile(rData)
{

}
Missile::~Missile()
{

}
void Missile::launch(const LaunchData& data)
{
	Projectile::launch(data);

	m_pTarget = getGame()->getUniverse().getChunk(data.targetChunkPosition);
	m_acceleration = data.acceleration;
	m_maxVelocity = data.maxVelocity;
}
void Missile::steer()
{
	Vec2 vel = m_body.getLinearVelocity();

	float totalVelocity = vel.len();
	float velAngle = Convert::normRad(atan2(vel.y, vel.x));

	float ourAngle = Convert::normRad(m_body.getAngle());

	if(totalVelocity < m_maxVelocity)
	{
		{ // torque direction
			float targetAngle = getTargetDirection(m_pTarget).toAngle();//angle of target
			float diffAngle = Convert::normRad(targetAngle - velAngle);//between velocity and target

			normalizeAngle(diffAngle);
			minimizeAngle(diffAngle);
			float objectiveAngle = Convert::normRad(diffAngle + targetAngle);//objective angle is what we want to face right now
			float diffObjectiveAngle = Convert::normRad(objectiveAngle - ourAngle);

			normalizeAngle(diffObjectiveAngle);

			int mod = 1;
			if(diffObjectiveAngle < 0)
				mod = -1;

			float torque = 30.f*mod* m_body.getInertia();

			m_body.applyTorque(torque);
		}

		{ // thrust direction
			Vec2 thrustDirection = Vec2::fromAngle(ourAngle);
			thrustDirection = thrustDirection.unit();
			thrustDirection *= m_body.getMass()*m_acceleration;

			m_body.applyForce(thrustDirection);
		}
	}
}
void Missile::prePhysUpdate()
{
	Projectile::prePhysUpdate();

	steer();
}
void Missile::postPhysUpdate()
{
	Projectile::postPhysUpdate();
}
void Missile::minimizeAngle(float& angle)
{
	if(angle < -Math::Pi / 5.f)
		angle = -Math::Pi / 5.f;
	else if(angle > Math::Pi / 5.f)
		angle = Math::Pi / 5.f;
}
void Missile::normalizeAngle(float& diffObjectiveAngle)
{
	if(diffObjectiveAngle > Math::Pi)
		diffObjectiveAngle -= 2 * Math::Pi;
}
Vec2 Missile::getTargetDirection(Chunk* target)
{
	Vec2 ourPos = m_body.getPosition();

	BodyComponent& targetBody = target->getBodyComponent();
	Vec2 targetVel = targetBody.getLinearVelocity();
	Vec2 targetPos = targetBody.getPosition();

	float dist = ourPos.to(targetPos).len();
	float eta = 3 * dist / m_maxVelocity;

	targetPos.x = targetPos.x + targetVel.x * eta;
	targetPos.y = targetPos.y + targetVel.y * eta;

	return targetPos - ourPos;
}
void MissileData::loadJson(const Json::Value& root)
{

	ProjectileData::loadJson(root);
}


