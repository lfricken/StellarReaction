#include "Projectile.hpp"
#include "ProjectileMan.hpp"
#include "Game.hpp"
#include "BlueprintLoader.hpp"

using namespace std;

Projectile::Projectile(const ProjectileData& rData) : m_body(rData.body), m_energyPool(rData.energyData), m_ballisticPool(rData.ballisticData), m_missilePool(rData.missileData), m_zoomPool(rData.zoomData)
{
	m_inPlay = false;
	m_timer.setCountDown(rData.lifetime);
	m_title = rData.title;
	PoolCollection myPools;
	myPools.ballisticPool = &m_ballisticPool;
	myPools.zoomPool = &m_zoomPool;
	myPools.missilePool = &m_missilePool;
	myPools.energyPool = &m_energyPool;


	std::vector<sptr<ModuleData> > thisData;
	for(auto it = rData.moduleData.begin(); it != rData.moduleData.end(); ++it)
		thisData.push_back(sptr<ModuleData>((*it)->clone()));

	for(auto it = thisData.begin(); it != thisData.end(); ++it)
		(*it)->ioComp.pMyManager = &game.getUniverse().getUniverseIO();

	for(auto it = thisData.begin(); it != thisData.end(); ++it)
		m_modules.push_back(sptr<ProjectileModule>(dynamic_cast<ProjectileModule*>((*it)->generate(m_body.getBodyPtr(), myPools, NULL))));
}
Projectile::~Projectile()
{

}
void Projectile::launch(const b2Vec2& rStart, const b2Vec2& rVel, float radCCW, float radCCWps, float lifetime, int damage, const FixtureComponent* pParent, int collisions)
{
	m_inPlay = true;
	m_timer.setCountDown(lifetime);
	m_timer.restartCountDown();
	//cout << m_body.getPosition().x << " " << m_body.getPosition().y;
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
const std::string& Projectile::getTitle() const
{
	return m_title;
}
void ProjectileData::loadJson(const Json::Value& root)
{
	if(!root["Title"].isNull())
		title = root["Title"].asString();

	if(!root["Body"].isNull())
		body.loadJson(root["Body"]);

	if(!root["Modules"].isNull())
	{
		sptr<ModuleData> spMod;
		for(auto it = root["Modules"].begin(); it != root["Modules"].end(); ++it)
		{
			if(!(*it)["Title"].isNull() && (*it)["ClassName"].isNull())//from title
			{
				spMod.reset(game.getUniverse().getBlueprints().getModuleSPtr((*it)["Title"].asString())->clone());

				spMod->fixComp.offset.x = (*it)["Position"][0].asFloat();
				spMod->fixComp.offset.y = (*it)["Position"][1].asFloat();
			}
			else if(!(*it)["ClassName"].isNull())//from inline
			{
				//inlining		spMod.reset(game.getUniverse().getBlueprints().loadModule(*it)->clone());
			}
			else
			{
				cout << "\n" << FILELINE;
				///ERROR LOG
			}

			moduleData.push_back(spMod);
		}
	}
}


