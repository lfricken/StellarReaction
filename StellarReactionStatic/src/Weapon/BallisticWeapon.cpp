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
	m_velocity = rData.velocity;
	m_projLifetime = rData.range / m_velocity;
}
BallisticWeapon::~BallisticWeapon()
{

}
void BallisticWeapon::preShot(const Vec2& center, const Vec2& aim, float radCCW, float module_orientation)
{
	/**Fire projectiles**/
	ProjectileMan* pProj = &game.getUniverse().getProjMan();

	const Vec2 dif = center.to(aim);
	const Vec2 direction = dif.unit();
	const float spin = 0;
	const float shotAngle = atan2(direction.y, direction.x);
	const Vec2 vel = direction * m_velocity;
	const int damagePerShot = m_damage / m_shots;

	Projectile::launch(pProj, center, vel, shotAngle, spin, m_projLifetime, damagePerShot, m_pTempParent, m_collisions);
}
void BallisticWeapon::postShot(const Vec2& center, const Vec2& aim, float radCCW, float module_orientation)
{

}