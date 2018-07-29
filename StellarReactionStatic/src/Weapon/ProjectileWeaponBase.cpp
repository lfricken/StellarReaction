#include "ProjectileWeaponBase.hpp"


void ProjectileWeaponBaseData::loadJson(const Json::Value& root)
{
	WeaponData::loadJson(root);

	GETJSON(projectileData.lifetime);
	GETJSON(projectileData.blueprint);
	GETJSON(projectileData.velocity);
}

ProjectileWeaponBase::ProjectileWeaponBase(const ProjectileWeaponBaseData& data) : Weapon(data)
{
	projectileData = data.projectileData;
}
ProjectileWeaponBase::~ProjectileWeaponBase()
{

}
