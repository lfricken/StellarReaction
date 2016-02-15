#include "MissileWeapon.hpp"
#include "Missile.hpp"
#include "ProjectileMan.hpp"

using namespace std;

MissileWeapon::MissileWeapon(const MissileWeaponData& rData) : Weapon(rData)
{
	m_projName = rData.projName;
	m_velocity = rData.velocity * sizeScalingFactor;
	m_projLifetime = rData.range * sizeScalingFactor / m_velocity;
}
MissileWeapon::~MissileWeapon()
{

}
void MissileWeapon::preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW)
{
	/**Fire projectiles**/
	Projectile* pProj = game.getUniverse().getProjMan().getProjectile(m_projName);

	b2Vec2 dif = aim - center;
	b2Vec2 vel = aim - center;
	vel.Normalize();
	vel *= m_velocity;
	dynamic_cast<Missile*>(pProj)->missileLaunch();///CHANGE FIRST 0 TO ANGLE of shot

}
void MissileWeapon::postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW)
{

}
