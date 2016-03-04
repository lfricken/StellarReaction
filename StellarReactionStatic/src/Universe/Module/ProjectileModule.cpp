#include "ProjectileModule.hpp"

using namespace std;

Register(ModuleData, ProjectileModuleData);


ProjectileModule::ProjectileModule(const ProjectileModuleData& rData) : Sensor(rData)
{
	m_freeThisProjectile = false;
	m_decors.push_back(sptr<GraphicsComponent>(new QuadComponent(rData.baseDecor)));
	m_decors.back()->setPosition(m_fix.getCenter());
	m_decors.back()->setRotation(m_fix.getAngle());
	m_pParent = NULL;
	m_currentCollisions = 0;
	m_maxCollisions = 1;
	m_damage = 0;
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
	m_damage = damage;
	m_currentCollisions = 0;
	m_maxCollisions = collisions;
	m_freeThisProjectile = false;
	m_pParent = pParent->getBodyPtr();//make sure the module we damage isn't from our own ship!
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
	if(pOther->getBodyPtr() != m_pParent && m_currentCollisions < m_maxCollisions)
	{
		damage(pOther, m_damage);
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
void ProjectileModule::damage(FixtureComponent* pFix, int damage)
{
	FixtureComponent& rComp = *pFix;
	sf::Packet packet;
	packet << m_damage;
	Message mess;
	mess.reset(rComp.getIOPos(), "damage", packet, 0.f, false);
	game.getUniverse().getUniverseIO().recieve(mess);
}
void ProjectileModuleData::loadJson(const Json::Value& root)
{
	if(!root["BaseSprite"].isNull())
		baseDecor.loadJson(root["BaseSprite"]);

	SensorData::loadJson(root);
}
