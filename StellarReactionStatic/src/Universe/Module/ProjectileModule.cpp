#include "ProjectileModule.hpp"
#include "Weapon.hpp"

using namespace std;

void ProjectileModuleData::loadJson(const Json::Value& root)
{
	LOADJSON(baseDecor);

	SensorData::loadJson(root);
}
ProjectileModule::ProjectileModule(const ProjectileModuleData& rData) : Sensor(rData)
{
	m_freeThisProjectile = false;
	m_decors.push_back(sptr<GraphicsComponent>(new QuadComponent(rData.baseDecor)));
	m_pParent = NULL;
	m_currentCollisions = 0;
	m_maxCollisions = 2;
	m_damage = 0;
	m_team = 0;
	m_sourceIOPos = -1;
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
void ProjectileModule::setPayload(int damage, const FixtureComponent* pParent, int collisions)
{
	const BodyComponent* pParentBody = static_cast<const BodyComponent*>(pParent->getBodyPtr()->GetUserData());

	m_currentCollisions = 0;
	m_freeThisProjectile = false;

	m_damage = damage;
	m_maxCollisions = collisions;

	m_team = pParentBody->getTeam();
	m_pParent = pParent->getBodyPtr();//make sure the module we damage isn't from our own ship!
	m_sourceIOPos = pParent->getIOPos();
}
void ProjectileModule::postPhysUpdate()
{
	for(int i = 0; i<(signed)m_decors.size(); ++i)
	{
		m_decors[i]->setPosition(m_fix.getCenter());
		m_decors[i]->setRotation(m_fix.getAngle());
	}
}
/// <summary>
/// when a fixture enters our sensor. called multiple times in a step if multiple bodies entered (in no particular order)
/// </summary>
void ProjectileModule::entered(FixtureComponent* pOther)
{
	if(pOther->getBodyPtr() != m_pParent && m_currentCollisions < m_maxCollisions && !pOther->isSensor())
	{
		Weapon::damage(&game.getUniverse().getUniverseIO(), pOther->getIOPos(), m_damage, m_sourceIOPos, m_team);

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
void ProjectileModule::input(std::string rCommand, sf::Packet rData)
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
