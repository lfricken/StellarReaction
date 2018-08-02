#include "ProjectileModule.hpp"
#include "Weapon.hpp"
#include "BodyComponent.hpp"



void ProjectileModuleData::loadJson(const Json::Value& root)
{
	LOADJSON(baseDecor);

	SensorData::loadJson(root);
}
ProjectileModule::ProjectileModule(const ProjectileModuleData& rData) : Sensor(rData)
{
	m_freeThisProjectile = false;
	m_decors.push_back(sptr<GraphicsComponent>(new QuadComponent(rData.baseDecor)));

	m_currentCollisions = 0;
	m_maxCollisions = 2;
	m_damage = 0;
	m_friendlyTeam = Team::Invalid;
	m_launcherModuleIoPosition = -1;
}
ProjectileModule::~ProjectileModule()
{

}
/// <summary>
/// Sets the payload.
/// </summary>
/// <param name="rPayload">Message to give fixtures we encounter</param>
/// <param name="pParent">to get body that we shouldn't collide with (because they are our parent)</param>
/// <param name="collisions">how many modules to damage</param>
void ProjectileModule::setPayload(int damage, Team team, int launcherModuleIoPosition, int collisions)
{
	m_currentCollisions = 0;
	m_freeThisProjectile = false;

	m_damage = damage;
	m_maxCollisions = collisions;

	m_friendlyTeam = team;
	m_launcherModuleIoPosition = launcherModuleIoPosition;
}
void ProjectileModule::postPhysUpdate()
{
	for(int i = 0; i < (signed)m_decors.size(); ++i)
	{
		m_decors[i]->setPosition(m_fix.getCenter());
		m_decors[i]->setRotation(m_fix.getParentBody()->getAngle());
	}
}
/// <summary>
/// when a fixture enters our sensor. called multiple times in a step if multiple bodies entered (in no particular order)
/// </summary>
void ProjectileModule::entered(FixtureComponent* pOther)
{
	Team collidingTeam = pOther->getParentBody()->getTeam();

	if(collidingTeam != m_friendlyTeam && m_currentCollisions < m_maxCollisions)
	{
		const Vec2 ourVelocity = m_parent->getBodyComponent().getLinearVelocity();
		const Vec2 theirVelocity = pOther->getParentBody()->getLinearVelocity();
		const Vec2 impactVelocity = ourVelocity - theirVelocity;

		Weapon::damage(&getGame()->getUniverse().getUniverseIO(), pOther->getIOPos(), m_damage, m_launcherModuleIoPosition, m_friendlyTeam, m_fix.getCenter(), impactVelocity, "LowSparks");

		++m_currentCollisions;
		if(m_currentCollisions >= m_maxCollisions)
			m_freeThisProjectile = true;
	}
}
/// <summary>
/// when a fixture exits our sensor. called multiple times in a step if multiple bodies entered (in no particular order)
/// </summary>
void ProjectileModule::exited(FixtureComponent* pOther)
{

}
bool ProjectileModule::shouldTerminate() const
{
	return m_freeThisProjectile;
}
void ProjectileModule::input(String rCommand, sf::Packet rData)
{
	if(rCommand == "damage")
	{

	}
	else if(rCommand == "hitShield")
	{
		m_freeThisProjectile = true;
	}
	else
		Module::input(rCommand, rData);
}
