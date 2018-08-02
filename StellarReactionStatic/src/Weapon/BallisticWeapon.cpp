#include "BallisticWeapon.hpp"
#include "Projectile.hpp"
#include "ProjectileMan.hpp"




void BallisticWeaponData::loadJson(const Json::Value& root)
{
	WeaponData::loadJson(root);
}
BallisticWeapon::BallisticWeapon(const BallisticWeaponData& rData) : ProjectileWeaponBase(rData)
{

}
BallisticWeapon::~BallisticWeapon()
{

}
void BallisticWeapon::preShot(const ShotData& data)
{
	/**Fire projectiles**/
	Projectile::LaunchData launchData;

	const Vec2 dif = data.weaponCenter.to(data.aim);
	const Vec2 direction = dif.unit();

	launchData.blueprint = projectileData.blueprint;
	launchData.targetChunkPosition = 0;
	launchData.launcherModuleIoPosition = m_pTempParent->getIOPos();

	launchData.startPosition = data.weaponCenter;

	launchData.rotation = atan2(direction.y, direction.x);
	launchData.rotationRate = 0;
	launchData.velocity = direction * projectileData.velocity;
	launchData.maxVelocity = 0.f;
	launchData.acceleration = 0.f;

	launchData.team = m_team;
	launchData.lifetime = projectileData.lifetime;
	launchData.damage = m_damagePerShot;
	launchData.collisions = m_collisions;

	getGame()->getUniverse().getProjMan().launchNewProjectile(launchData);
}
void BallisticWeapon::postShot(const ShotData& data)
{

}