#include "MissileWeapon.hpp"
#include "Missile.hpp"
#include "ProjectileMan.hpp"
#include "Chunk.hpp"

void MissileWeaponBlueprint::loadJson(const Json::Value& root)
{
	ProjectileWeaponBaseData::loadJson(root);

	GETJSON(missileData.alignWithAim);
	GETJSON(missileData.alignAngle);

	GETJSON(missileData.launchVelocityWithAim);
	GETJSON(missileData.launchVelocityAngle);

	GETJSON(missileData.acceleration);
	GETJSON(missileData.maxVelocity);
}
MissileWeapon::MissileWeapon(const MissileWeaponBlueprint& data) : ProjectileWeaponBase(data)
{
	missileData = data.missileData;
}
MissileWeapon::~MissileWeapon()
{

}
void MissileWeapon::preShot(const ShotData& data)
{
	ProjectileMan* manager = &getGame()->getUniverse().getProjMan();

	Chunk* target = getGame()->getUniverse().getNearestChunk(data.aim);
	if(target != nullptr)
	{
		Vec2 computedVelocity;
		float computedRotation;
		const Vec2 shipDirection = Vec2::fromAngle(data.shipRotation).unit();

		if(missileData.launchVelocityWithAim)
		{
			computedVelocity = data.aim.unit();
			computedVelocity *= projectileData.velocity;
		}
		else
		{
			computedVelocity = shipDirection.rotate(missileData.launchVelocityAngle);
			computedVelocity *= projectileData.velocity;
		}

		if(missileData.alignWithAim)
		{
			computedRotation = data.weaponCenter.to(data.aim).toAngle();
		}
		else
		{
			computedRotation = data.shipRotation + missileData.alignAngle;
		}


		/**Fire projectiles**/
		Projectile::LaunchData launchData;

		launchData.blueprint = projectileData.blueprint; 
		launchData.targetChunkPosition = target->getFactoryPosition();
		launchData.launcherModuleIoPosition = m_pTempParent->getIOPos();

		launchData.startPosition = data.weaponCenter;

		launchData.rotation = computedRotation;
		launchData.rotationRate = 0;
		launchData.velocity = computedVelocity;
		launchData.maxVelocity = missileData.maxVelocity;
		launchData.acceleration = missileData.acceleration;

		launchData.team = m_team;
		launchData.lifetime = projectileData.lifetime;
		launchData.damage = m_damagePerShot;
		launchData.collisions = m_collisions;

		getGame()->getUniverse().getProjMan().launchNewProjectile(launchData);
	}
}
void MissileWeapon::postShot(const ShotData& data)
{

}


