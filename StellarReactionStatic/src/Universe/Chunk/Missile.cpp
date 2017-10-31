#include "Missile.hpp"
#include "Convert.hpp"




Missile::Missile(const MissileData& rData) : Projectile(rData)
{

}
Missile::~Missile()
{

}
void Missile::missileLaunch(Vec2 rStart, wptr<Chunk> target, float module_orientation, float init_velocity, float acceleration, float max_velocity, int damage, const FixtureComponent* pParent, int collisions)
{
	m_pTarget = target;
	m_acceleration = acceleration * 2;
	m_maxVelocity = max_velocity;

	Vec2 inititalDirection(cos(module_orientation), sin(module_orientation));
	inititalDirection = inititalDirection.unit();
	inititalDirection *= init_velocity;

	m_inPlay = true;
	m_timer.setCountDown(10);
	m_timer.restartCountDown();
	m_body.wake(rStart, module_orientation, inititalDirection, 0);

	setPayloadOnModules(damage, pParent, collisions);
}
void Missile::steer()
{
	b2Body& bod = *m_body.getBodyPtr();
	Vec2 vel = bod.GetLinearVelocity();

	float totalVelocity = vel.len();
	float velAngle = leon::normRad(atan2(vel.y, vel.x));

	float ourAngle = leon::normRad(bod.GetAngle());

	if(totalVelocity < m_maxVelocity)
	{
		if(auto target = m_pTarget.lock())
		{
			Vec2 direction = getTargetDirection(target);

			float targetAngle = leon::normRad(atan2(direction.y, direction.x));//angle of target
			float diffAngle = leon::normRad(targetAngle - velAngle);//between velocity and target

			normalizeAngle(diffAngle);
			minimizeAngle(diffAngle);
			float objectiveAngle = leon::normRad(diffAngle + targetAngle);//objective angle is what we want to face right now
			float diffObjectiveAngle = leon::normRad(objectiveAngle - ourAngle);

			normalizeAngle(diffObjectiveAngle);

			int mod = 1;
			if(diffObjectiveAngle < 0)
				mod = -1;

			float torque = 30.f*mod*bod.GetInertia();

			bod.ApplyTorque(torque, true);

		}

		Vec2 direction(cos(ourAngle), sin(ourAngle));
		direction = direction.unit();
		direction *= bod.GetMass()*m_acceleration;

		bod.ApplyForceToCenter(direction, true);
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
	if(angle < -pi / 5.f)
		angle = -pi / 5.f;
	else if(angle > pi / 5.f)
		angle = pi / 5.f;
}
void Missile::normalizeAngle(float& diffObjectiveAngle)
{
	if(diffObjectiveAngle > pi)
		diffObjectiveAngle -= 2 * pi;
}
Vec2 Missile::getTargetDirection(sptr<Chunk> target)
{
	b2Body& bod = *m_body.getBodyPtr();
	Vec2 ourPos = bod.GetPosition();
	b2Body& tBod = *target->getBodyComponent().getBodyPtr();
	Vec2 targetVel = tBod.GetLinearVelocity();
	Vec2 targetPos = tBod.GetPosition();
	Vec2 diff = targetPos - ourPos;

	float dist = diff.len();
	float eta = 3 * dist / m_maxVelocity;

	targetPos.x = targetPos.x + targetVel.x * eta;
	targetPos.y = targetPos.y + targetVel.y * eta;

	return targetPos - ourPos;
}
void MissileData::loadJson(const Json::Value& root)
{

	ProjectileData::loadJson(root);
}


