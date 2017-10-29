#include "Projectile.hpp"
#include "ProjectileMan.hpp"
#include "Game.hpp"
#include "BlueprintLoader.hpp"




void ProjectileData::loadJson(const Json::Value& root)
{
	GETJSON(title);

	LOADJSON(body);

	if(!root["Modules"].isNull())
	{
		sptr<ModuleData> spMod;
		for(auto it = root["Modules"].begin(); it != root["Modules"].end(); ++it)
		{
			if(!(*it)["title"].isNull() && (*it)["ClassName"].isNull())//from title
			{
				spMod.reset(game.getUniverse().getBlueprints().getModuleSPtr((*it)["title"].asString())->clone());

				spMod->fixComp.offset.x = (*it)["Position"][0].asFloat();
				spMod->fixComp.offset.y = (*it)["Position"][1].asFloat();
			}
			else
			{
				WARNING;
			}

			moduleData.push_back(spMod);
		}
	}
}
Projectile::Projectile(const ProjectileData& data) : ModuleParent(data), m_body(data.body), ranges(data.rangesData)
{
	ModuleData::GenerateParams params;
	params.parent = this;

	m_inPlay = false;
	m_timer.setCountDown(data.lifetime);
	m_title = data.title;


	List<sptr<ModuleData> > thisData;
	for(auto it = data.moduleData.begin(); it != data.moduleData.end(); ++it)
		thisData.push_back(sptr<ModuleData>((*it)->clone()));

	for(auto it = thisData.begin(); it != thisData.end(); ++it)
		(*it)->ioComp.pMyManager = &game.getUniverse().getUniverseIO();

	for(auto it = thisData.begin(); it != thisData.end(); ++it)
		m_modules.push_back(sptr<ProjectileModule>(dynamic_cast<ProjectileModule*>((*it)->generate(params))));
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
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->setPayload(damage, pParent, collisions);
}
void Projectile::reset()//this projectile will go back into projectile pool
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
				if((*it)->shouldTerminate())
					endThyself = true;
		if(endThyself)
			reset();

		//update graphics
		for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
			(*it)->postPhysUpdate();
	}
	//update hull
}
const String& Projectile::getTitle() const
{
	return m_title;
}


