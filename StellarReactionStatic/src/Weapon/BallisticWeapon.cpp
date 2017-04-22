#include "BallisticWeapon.hpp"
#include "Projectile.hpp"
#include "ProjectileMan.hpp"




void BallisticWeaponData::loadJson(const Json::Value& root)
{
	WeaponData::loadJson(root);

	GETJSON(projName);
	GETJSON(velocity);
}
BallisticWeapon::BallisticWeapon(const BallisticWeaponData& rData) : Weapon(rData)
{
	m_projName = rData.projName;
	m_velocity = rData.velocity * sizeScalingFactor;
	m_projLifetime = rData.range * sizeScalingFactor / m_velocity;
}
BallisticWeapon::~BallisticWeapon()
{

}
void BallisticWeapon::preShot(const Vec2& center, const Vec2& aim, float radCCW, float module_orientation)
{
	/**Fire projectiles**/
	Projectile* pProj = game.getUniverse().getProjMan().getProjectile(m_projName);

	Vec2 dif = aim - center;
	Vec2 vel = aim - center;
	vel = vel.unit();
	vel *= m_velocity;
	pProj->launch(center, vel, atan2(dif.y, dif.x), 0, m_projLifetime, m_damage/m_shots, m_pTempParent, m_collisions);///CHANGE FIRST 0 TO ANGLE of shot

}
void BallisticWeapon::postShot(const Vec2& center, const Vec2& aim, float radCCW, float module_orientation)
{

}