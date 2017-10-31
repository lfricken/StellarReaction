#include "Projectile.hpp"
#include "ProjectileMan.hpp"
#include "Game.hpp"
#include "BlueprintLoader.hpp"


void ProjectileData::loadJson(const Json::Value& root)
{
	ModuleParentData::loadJson(root);

	GETJSON(lifetime);
}
Projectile::Projectile(const ProjectileData& data) : ModuleParent(data)
{
	m_inPlay = false;
	m_timer.setCountDown(data.lifetime);
}
Projectile::~Projectile()
{

}
void Projectile::launch(const Vec2& rStart, const Vec2& rVel, float radCCW, float radCCWps, float lifetime, int damage, const FixtureComponent* pParent, int collisions)
{
	m_inPlay = true;
	m_timer.setCountDown(lifetime);
	m_timer.restartCountDown();
	m_body.wake(rStart, radCCW, rVel, radCCWps);

	setPayloadOnModules(damage, pParent, collisions);
}
void Projectile::setPayloadOnModules(int damage, const FixtureComponent* pParent, int collisions)
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
	{
		ProjectileModule* mod = dynamic_cast<ProjectileModule*>(it->get());
		if(mod != nullptr)
			mod->setPayload(damage, pParent, collisions);
		else
			WARNING;
	}
}
void Projectile::reset()
{
	m_inPlay = false;
	game.getUniverse().getProjMan().freeProjectile(this);
	m_body.sleep();
}
void Projectile::prePhysUpdate()
{
	if(m_inPlay)
		for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
			(*it)->prePhysUpdate();
	//update hull
}
void Projectile::postPhysUpdate()
{
	if(m_inPlay)
	{
		//check to see if we should terminate
		bool endThyself = false;

		if(m_timer.isTimeUp())
			endThyself = true;
		else
			for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
			{
				ProjectileModule* mod = dynamic_cast<ProjectileModule*>(it->get());
				if(mod != nullptr)
					if(mod->shouldTerminate())
						endThyself = true;
				else
					WARNING;
			}

		if(endThyself)
			reset();

		//update graphics
		for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
			(*it)->postPhysUpdate();
	}
	//update hull
}


