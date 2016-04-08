#include "Chunk.hpp"
#include "Module.hpp"
#include "SlaveLocator.hpp"
#include "Controller.hpp"
#include "Overlay.hpp"
#include "BlueprintLoader.hpp"
#include "SoundManager.hpp"
#include "Animation.hpp"
#include "ShipModule.hpp"
#include "CommandInfo.hpp"

using namespace std;

void ChunkData::loadJson(const Json::Value& root)
{
	LOADJSON(ioComp);
	LOADJSON(nwComp);
	LOADJSON(bodyComp);
	LOADJSONT(energyData, Energy);
	LOADJSONT(ballisticData, Ballistic);
	LOADJSONT(missileData, Missiles);
	LOADJSONT(zoomData, Zoom);
	LOADJSON(hullSpriteData);

	if(!root["afterburnerSpriteData"].isNull())
		for(auto it = root["afterburnerSpriteData"].begin(); it != root["afterburnerSpriteData"].end(); ++it)
		{
			QuadComponentData quad;
			quad.loadJson(*it);
			afterburnerSpriteData.push_back(quad);
		}

	if(!root["afterburnerThrustSpriteData"].isNull())
		for(auto it = root["afterburnerThrustSpriteData"].begin(); it != root["afterburnerThrustSpriteData"].end(); ++it)
		{
			QuadComponentData quad;
			quad.loadJson(*it);
			afterburnerThrustSpriteData.push_back(quad);
		}

	if(!root["moduleData"].isNull())
	{
		sptr<ModuleData> spMod;
		for(auto it = root["moduleData"].begin(); it != root["moduleData"].end(); ++it)
		{
			if(!(*it)["title"].isNull() && (*it)["ClassName"].isNull())//from title
			{
				string title = (*it)["title"].asString();
				spMod.reset(game.getUniverse().getBlueprints().getModuleSPtr(title)->clone());

				spMod->fixComp.offset.x = (*it)["Position"][0].asFloat();
				spMod->fixComp.offset.y = (*it)["Position"][1].asFloat();
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
Chunk::Chunk(const ChunkData& rData) : GameObject(rData), m_body(rData.bodyComp), m_zoomPool(rData.zoomData), m_energyPool(rData.energyData), m_ballisticPool(rData.ballisticData), m_missilePool(rData.missileData)
{
	m_spawnPoint = b2Vec2(0, 0);
	m_deaths = 0;
	m_wasThrusting = false;
	m_wasBoosting = false;
	m_stealth = false;
	m_timer.setCountDown(1);
	m_timer.restartCountDown();
	PoolCollection myPools;
	myPools.ballisticPool = &m_ballisticPool;
	myPools.zoomPool = &m_zoomPool;
	myPools.missilePool = &m_missilePool;
	myPools.energyPool = &m_energyPool;

	m_validOffsets = rData.validPos;

	//Make a copy 
	std::vector<sptr<ModuleData> > thisData;
	for(auto it = rData.moduleData.begin(); it != rData.moduleData.end(); ++it)
		thisData.push_back(sptr<ModuleData>((*it)->clone()));

	for(auto it = thisData.begin(); it != thisData.end(); ++it)
		(*it)->ioComp.pMyManager = rData.ioComp.pMyManager;

	for(auto it = thisData.begin(); it != thisData.end(); ++it)
		m_modules.push_back(sptr<Module>((*it)->generate(m_body.getBodyPtr(), myPools, this)));

	m_slavePosition = m_rParent.getSlaveLocator().give(this);



	hull = sptr<GraphicsComponent>(new QuadComponent(rData.hullSpriteData));
	hull->setPosition(m_body.getPosition());
	hull->setRotation(m_body.getBodyPtr()->GetAngle());
	for(auto it = rData.afterburnerSpriteData.begin(); it != rData.afterburnerSpriteData.end(); ++it)
	{
		sptr<GraphicsComponent> temp = sptr<GraphicsComponent>(new QuadComponent(*it));
		temp->setPosition(m_body.getPosition());
		temp->setRotation(m_body.getBodyPtr()->GetAngle());
		afterburners.push_back(temp);
	}
	for(auto it = rData.afterburnerThrustSpriteData.begin(); it != rData.afterburnerThrustSpriteData.end(); ++it)
	{
		sptr<GraphicsComponent> temp = sptr<GraphicsComponent>(new QuadComponent(*it));
		temp->setPosition(m_body.getPosition());
		temp->setRotation(m_body.getBodyPtr()->GetAngle());
		afterburners_boost.push_back(temp);
	}

	m_thrustNoiseIndex = -1;
	m_boostNoiseIndex = -1;
	m_body.setTeam(rData.team);
}
Chunk::~Chunk()
{
	m_rParent.getSlaveLocator().free(m_slavePosition);
}

int Chunk::incDeaths()
{
	return m_deaths++;
}
b2Vec2 Chunk::getSpawn()
{
	return m_spawnPoint;
}
void Chunk::setStealth(bool stealthToggle)
{
	m_stealth = stealthToggle;
}
bool Chunk::isStealth()
{
	return m_stealth;
}
sptr<GraphicsComponent> Chunk::getHull() const
{
	return hull;
}
std::vector<sptr<Module>> Chunk::getModuleList() const
{
	return m_modules;
}
bool Chunk::allows(const b2Vec2& rGridPos)
{
	return (std::find(m_validOffsets.begin(), m_validOffsets.end(), rGridPos) != m_validOffsets.end());
}
BodyComponent& Chunk::getBodyComponent()
{
	return m_body;
}
int Chunk::getScore()
{
	return m_score;
}
void Chunk::increaseScore()
{
	m_score++;
}
void Chunk::add(const ModuleData& rData)
{
	if(this->allows(rData.fixComp.offset))
	{
		PoolCollection myPools;
		myPools.ballisticPool = &m_ballisticPool;
		myPools.zoomPool = &m_zoomPool;
		myPools.missilePool = &m_missilePool;
		myPools.energyPool = &m_energyPool;
		m_modules.push_back(sptr<Module>(rData.generate(m_body.getBodyPtr(), myPools, this)));
	}
	else
		cout << FILELINE;
}
void Chunk::prePhysUpdate()
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->prePhysUpdate();
}
void Chunk::postPhysUpdate()
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->postPhysUpdate();

	m_nw.toggleNewData(true);
	m_body.getNWComp().toggleNewData(true);

	//Evan - rotate hull, afterburner and afterburner_thrust
	hull->setPosition(m_body.getPosition());
	hull->setRotation(m_body.getBodyPtr()->GetAngle());
	for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
	{
		(*it)->setPosition(m_body.getPosition());
		(*it)->setRotation(m_body.getBodyPtr()->GetAngle());
	}
	for(auto it = afterburners_boost.begin(); it != afterburners_boost.end(); ++it)
	{
		(*it)->setPosition(m_body.getPosition());
		(*it)->setRotation(m_body.getBodyPtr()->GetAngle());
	}
}
const std::string& Chunk::getName() const
{
	return m_io.getName();
}
void Chunk::setAim(const b2Vec2& world)//send our aim coordinates
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->setAim(world);
}
void Chunk::directive(const CommandInfo& commands)//send command to target
{
	std::map<Directive, bool> rIssues = commands.directives;
	bool local = commands.isLocal;

	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->directive(commands);

	if(rIssues[Directive::ShowStore] && local)
	{
		if(m_timer.isTimeUp())
		{
			std::string store;
			for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
			{
				store = (*it)->getStore();
				if(store != "")
					break;
			}
			if(store != "")
			{
				Message toggle(store, "toggleHidden", voidPacket, 0, false);
				Message mes2("local_player", "toggleGuiMode", voidPacket, 0, false);
				game.getCoreIO().recieve(toggle);
				game.getCoreIO().recieve(mes2);
			}
			m_timer.restartCountDown();
		}
	}

	bool startThrusting = (rIssues[Directive::Up] && !m_wasThrusting);
	bool startBoosting = (rIssues[Directive::Up] && rIssues[Directive::Boost] && !m_wasBoosting);



	if(startThrusting)
	{
		for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
			(*it)->getAnimator().setAnimation("Thrust", 0.20f);
		m_thrustNoiseIndex = game.getSound().playSound("Thruster.wav", 60, 20, 20, getBodyPtr()->GetPosition(), true, true);
	}
	if(startBoosting)
	{
		for(auto it = afterburners_boost.begin(); it != afterburners_boost.end(); ++it)
			(*it)->getAnimator().setAnimation("Boost", 0.20f);
		m_boostNoiseIndex = game.getSound().playSound("Afterburner.wav", 100, 20, 20, getBodyPtr()->GetPosition(), true, true);
	}



	if(!rIssues[Directive::Up])
	{
		for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
			(*it)->getAnimator().setAnimation("Default", 1.0f);
		m_thrustNoiseIndex = game.getSound().stopSound(m_thrustNoiseIndex);
	}
	if(!rIssues[Directive::Up] || !rIssues[Directive::Boost])
	{
		for(auto it = afterburners_boost.begin(); it != afterburners_boost.end(); ++it)
			(*it)->getAnimator().setAnimation("Default", 1.0f);
		m_boostNoiseIndex = game.getSound().stopSound(m_boostNoiseIndex);
	}

	m_wasThrusting = rIssues[Directive::Up];
	m_wasBoosting = (rIssues[Directive::Up] && rIssues[Directive::Boost]);
}
float Chunk::get(Request value) const//return the requested value
{
	switch(value)
	{
	case(Request::Zoom) :
		return static_cast<float>(m_zoomPool.getValue());
	case(Request::MaxZoom) :
		return static_cast<float>(m_zoomPool.getMax());


	case(Request::Energy) :
		return static_cast<float>(m_energyPool.getValue());
	case(Request::MaxEnergy) :
		return static_cast<float>(m_energyPool.getMax());


	case(Request::Ballistics) :
		return static_cast<float>(m_ballisticPool.getValue());
	case(Request::MaxBallistics) :
		return static_cast<float>(m_ballisticPool.getMax());


	case(Request::Missiles) :
		return static_cast<float>(m_missilePool.getValue());
	case(Request::MaxMissiles) :
		return static_cast<float>(m_missilePool.getMax());


	case(Request::Score) :
		return static_cast<float>(m_score);


	default:
		return 0.f;
		break;
	}
}
/// <summary>
/// returns the title of the module at that position, otherwise ""
/// </summary>
std::string Chunk::hasModuleAt(const b2Vec2 offset) const
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		if(offset == (*it)->getOffset())
			return (*it)->getTitle();
	return "";
}
/// <summary>
/// List of module BP names and 
/// </summary>
std::vector<std::pair<std::string, b2Vec2> > Chunk::getModules() const
{
	std::vector<std::pair<std::string, b2Vec2> > list;
	for(auto it = m_modules.cbegin(); it != m_modules.cend(); ++it)
	{
		if(dynamic_cast<ShipModule*>(it->get()) != NULL)//make sure it's not a strange item, like a ShieldComponent
		{
			//cout << "\nChunk: " << (*it)->getOffset().x << (*it)->getOffset().y;
			list.push_back(std::pair<std::string, b2Vec2>((*it)->getTitle(), b2Vec2((*it)->getOffset())));
		}
	}
	return list;
}
b2Body* Chunk::getBodyPtr()
{
	return m_body.getBodyPtr();
}
void Chunk::clear()
{
	m_modules.clear();
}
void Chunk::input(std::string rCommand, sf::Packet rData)
{
	if(rCommand == "clear")
	{
		this->clear();
	}
	else if(rCommand == "attachModule")
	{
		std::string bpName;
		float x;
		float y;

		rData >> bpName;
		rData >> x;
		rData >> y;

		sptr<ModuleData> pNewModuleData = sptr<ModuleData>(m_rParent.getBlueprints().getModuleSPtr(bpName)->clone());
		if(pNewModuleData != NULL)
		{
			pNewModuleData->fixComp.offset = b2Vec2(x, y);
			this->add(*pNewModuleData);
		}
		else
		{
			cout << "\nBlueprint didn't exist." << FILELINE;
		}
	}
	else if(rCommand == "detachModule")
	{
		float x;
		float y;

		rData >> x;
		rData >> y;

		b2Vec2 targetOffset(x, y);//the offset we are looking to remove

		bool found = false;
		for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		{
			if(targetOffset == (*it)->getOffset())
			{
				found = true;
				m_modules.erase(it);
				break;
			}
		}
		if(!found)
			cout << "\nThere was no module at " << x << "," << y << " " << FILELINE;
	}
	else
		cout << "\nCommand not found in [" << m_io.getName() << "]." << FILELINE;
}
float Chunk::getRadius()
{
	b2Vec2 max = b2Vec2_zero;
	for (auto it = m_modules.cbegin(); it != m_modules.cend(); ++it)
	{
		if (max.Length() < b2Vec2((*it)->getOffset()).Length()){
			max = b2Vec2((*it)->getOffset());
		}
	}

	return max.Length();
}

