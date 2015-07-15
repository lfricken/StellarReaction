#include "Projectile.hpp"

Projectile::Projectile(const ProjectileData& rData) : m_body(rData.body), m_module(rData.module)
{

}
Projectile::~Projectile()
{

}
void Projectile::launch(const b2Vec2& rStart, const b2Vec2& rVel, float radCCW, float radCCWps, const Message& rPayload)
{
    m_body.wake(rStart, radCCW, rVel, radCCW);
	m_module.arm(rPayload);
}
void Projectile::reset()//this projectile will go back into projectile pool
{

}
