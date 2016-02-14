#include "Missile.hpp"

using namespace std;

Missile::Missile(const MissileData& rData) : Projectile(rData)
{

}
Missile::~Missile()
{

}
void Missile::missileLaunch(b2Vec2 rStart, Chunk* target, float module_orientation, int init_velocity, int acceleration, int max_velocity, int damage, const FixtureComponent* pParent, int collisions)
{
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
}

void Missile::postPhysUpdate()
{
	Projectile::postPhysUpdate();
}