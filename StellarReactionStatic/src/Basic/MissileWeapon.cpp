#include "MissileWeapon.hpp"
#include "Missile.hpp"
#include "ProjectileMan.hpp"
#include "Chunk.hpp"

using namespace std;

MissileWeapon::MissileWeapon(const MissileWeaponData& rData) : Weapon(rData)
{
	m_projName = rData.projName;

	m_acceleration = 30;
	m_max_velocity = 70;
	m_init_velocity = 2;

	m_velocity = rData.velocity * sizeScalingFactor;
	m_projLifetime = rData.range * sizeScalingFactor / m_velocity;
}
MissileWeapon::~MissileWeapon()
{

}
void MissileWeapon::preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation)
{
	/**Fire projectiles**/
	Projectile* pProj = game.getUniverse().getProjMan().getProjectile(m_projName);

	BodyComponent* target = game.getUniverse().getNearestBody(aim);
	dynamic_cast<Missile*>(pProj)->missileLaunch(center, target, module_orientation, m_init_velocity, m_acceleration, m_max_velocity, m_damage / m_shots, m_pTempParent, m_collisions);

}
void MissileWeapon::postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation)
{

}