#include "MissileWeapon.hpp"
#include "Missile.hpp"
#include "ProjectileMan.hpp"
#include "Chunk.hpp"




void MissileWeaponData::loadJson(const Json::Value& root)
{
	WeaponData::loadJson(root);

	GETJSON(projName);
}
MissileWeapon::MissileWeapon(const MissileWeaponData& rData) : Weapon(rData)
{
	m_projName = rData.projName;

	m_acceleration = 30.f;
	m_max_velocity = 70.f;
	m_init_velocity = 2.f;

	m_velocity = rData.velocity;
	m_projLifetime = rData.range / m_velocity;
}
MissileWeapon::~MissileWeapon()
{

}
void MissileWeapon::preShot(const Vec2& center, const Vec2& aim, float radCCW, float module_orientation)
{
	/**Fire projectiles**/
	Projectile* pProj = game.getUniverse().getProjMan().getProjectile(m_projName);

	auto target = game.getUniverse().getNearestChunk(aim);
	if(auto chunk = target.lock())
	{
		dynamic_cast<Missile*>(pProj)->missileLaunch(center, target, module_orientation, m_init_velocity, m_acceleration, m_max_velocity, m_damage / m_shots, m_pTempParent, m_collisions);
	}
}
void MissileWeapon::postShot(const Vec2& center, const Vec2& aim, float radCCW, float module_orientation)
{

}


