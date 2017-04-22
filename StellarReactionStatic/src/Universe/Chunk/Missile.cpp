#include "Missile.hpp"
#include "Convert.hpp"




Missile::Missile(const MissileData& rData) : Projectile(rData)
{
	m_pTarget = NULL;
}
Missile::~Missile()
{

}
void Missile::missileLaunch(Vec2 rStart, BodyComponent* target, float module_orientation, float init_velocity, float acceleration, float max_velocity, int damage, const FixtureComponent* pParent, int collisions)
{
	m_pTarget = target;
	m_acceleration = acceleration * 2;
	m_maxVelocity = max_velocity;

	m_inPlay = true;
	m_timer.setCountDown(10);
	m_timer.restartCountDown();

	Vec2 init_vec(cos(module_orientation), sin(module_orientation));
	init_vec = init_vec.unit();
	init_vec *= init_velocity;

	m_body.wake(rStart, module_orientation, init_vec, 0);
	for (auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->setPayload(damage, pParent, collisions);
}

void Missile::prePhysUpdate()
{
	Projectile::prePhysUpdate();
	b2Body& bod = *m_body.getBodyPtr();
	Vec2 vel = bod.GetLinearVelocity();

	float totalVelocity = vel.len();
	float velAngle = leon::normRad(atan2(vel.y, vel.x));

	float ourAngle = leon::normRad(bod.GetAngle());

	if(totalVelocity < m_maxVelocity)
	{
		if(m_pTarget != NULL)
		{
			Vec2 diff = getTargetPos();

			float targetAngle = leon::normRad(atan2(diff.y, diff.x));//angle of target
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
Vec2 Missile::getTargetPos()
{
	b2Body& bod = *m_body.getBodyPtr();
	Vec2 ourPos = bod.GetPosition();
	b2Body& tBod = *m_pTarget->getBodyPtr();
	Vec2 targetVel = tBod.GetLinearVelocity();
	Vec2 targetPos = tBod.GetPosition();
	Vec2 diff = targetPos - ourPos;

	float dist = diff.len();
	float eta = 3 * dist / m_maxVelocity;

	targetPos.x = targetPos.x + targetVel.x * eta;
	targetPos.y = targetPos.y + targetVel.y * eta;

	return targetPos - ourPos;
}
void Missile::postPhysUpdate()
{
	Projectile::postPhysUpdate();
}
void MissileData::loadJson(const Json::Value& root)
{

	ProjectileData::loadJson(root);
}


