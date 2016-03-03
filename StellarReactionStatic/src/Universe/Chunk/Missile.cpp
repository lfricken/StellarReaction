#include "Missile.hpp"
#include "Convert.hpp"

using namespace std;

Register(ProjectileData, MissileData);
Missile::Missile(const MissileData& rData) : Projectile(rData)
{
	m_pTarget = NULL;
}
Missile::~Missile()
{

}
void Missile::missileLaunch(b2Vec2 rStart, BodyComponent* target, float module_orientation, float init_velocity, float acceleration, float max_velocity, int damage, const FixtureComponent* pParent, int collisions)
{
	m_pTarget = target;
	m_acceleration = acceleration;
	m_maxVelocity = max_velocity;

	m_inPlay = true;
	m_timer.setCountDown(10);
	m_timer.restartCountDown();

	b2Vec2 init_vec(cos(module_orientation), sin(module_orientation));
	init_vec.Normalize();
	init_vec *= init_velocity;

	m_body.wake(rStart, module_orientation, init_vec, 0);
	for (auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->setPayload(damage, pParent, collisions);
}

void Missile::prePhysUpdate()
{
	Projectile::prePhysUpdate();
	b2Body& bod = *m_body.getBodyPtr();
	b2Vec2 vel = bod.GetLinearVelocity();

	float totalVelocity = vel.Length();
	float velAngle = leon::normRad(atan2(vel.y, vel.x));

	b2Vec2 ourPos = bod.GetPosition();
	float ourAngle = leon::normRad(bod.GetAngle());

	if(totalVelocity < m_maxVelocity)
	{
		if(m_pTarget != NULL)
		{
			b2Body& tBod = *m_pTarget->getBodyPtr();
			b2Vec2 targetVel = tBod.GetLinearVelocity();
			b2Vec2 targetPos = tBod.GetPosition();
			b2Vec2 diff = targetPos - ourPos;

			float dist = diff.Length();
			float eta = 3 * dist / m_maxVelocity;

			targetPos.x = targetPos.x + targetVel.x * eta;
			targetPos.y = targetPos.y + targetVel.y * eta;

			diff = targetPos - ourPos;

			float targetAngle = leon::normRad(atan2(diff.y, diff.x));//angle of target
			float diffAngle = leon::normRad(targetAngle - velAngle);//between velocity and target
			if(diffAngle > pi)
				diffAngle -= 2 * pi;


			if(diffAngle < -pi / 5.f)
				diffAngle = -pi / 5.f;
			else if(diffAngle > pi / 5.f)
				diffAngle = pi / 5.f;

			float objectiveAngle = leon::normRad(diffAngle + targetAngle);//objective angle is what we want to face right now

			float diffObjectiveAngle = leon::normRad(objectiveAngle - ourAngle);

			if(diffObjectiveAngle > pi)
				diffObjectiveAngle -= 2 * pi;

			int mod = 1;
			if(diffObjectiveAngle < 0)
				mod = -1;

			float torque = 0.09*bod.GetMass()*mod;

			bod.ApplyTorque(torque, true);

		}

		b2Vec2 direction(cos(ourAngle), sin(ourAngle));
		direction.Normalize();
		direction *= bod.GetMass()*m_acceleration;

		bod.ApplyForceToCenter(direction, true);
	}


}
void Missile::postPhysUpdate()
{
	Projectile::postPhysUpdate();
}
void MissileData::loadJson(const Json::Value& root)
{

	ProjectileData::loadJson(root);
}


