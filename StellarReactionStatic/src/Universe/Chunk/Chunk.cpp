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
	GETJSON(team);
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
				String title = (*it)["title"].asString();
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
	m_body.setTeam(rData.team);//important that team is set before module creation
	m_spawnPoint = m_body.getBodyPtr()->GetPosition();
	m_radius = -1.f;
	m_deaths = 0;
	m_wasThrusting = false;
	m_wasBoosting = false;
	m_stealth = false;
	m_timer.setCountDown(1);
	m_timer.restartCountDown();


	//Set valid module positions.
	m_validOffsets = rData.validPos;

	//Add Modules.
	for(auto it = rData.moduleData.cbegin(); it != rData.moduleData.cend(); ++it)
		add(**it);

	//Register for controlling.
	m_slavePosition = m_rParent.getSlaveLocator().give(this);


	//Hull Sprite
	hull = sptr<GraphicsComponent>(new QuadComponent(rData.hullSpriteData));
	hull->setPosition(m_body.getPosition());
	hull->setRotation(m_body.getBodyPtr()->GetAngle());
	//Thrust sprite.
	for(auto it = rData.afterburnerSpriteData.begin(); it != rData.afterburnerSpriteData.end(); ++it)
	{
		sptr<GraphicsComponent> temp = sptr<GraphicsComponent>(new QuadComponent(*it));
		temp->setPosition(m_body.getPosition());
		temp->setRotation(m_body.getBodyPtr()->GetAngle());
		afterburners.push_back(temp);
	}
	//Boost sprite.
	for(auto it = rData.afterburnerThrustSpriteData.begin(); it != rData.afterburnerThrustSpriteData.end(); ++it)
	{
		sptr<GraphicsComponent> temp = sptr<GraphicsComponent>(new QuadComponent(*it));
		temp->setPosition(m_body.getPosition());
		temp->setRotation(m_body.getBodyPtr()->GetAngle());
		afterburners_boost.push_back(temp);
	}

	//For sounds.
	m_thrustNoiseIndex = -1;
	m_boostNoiseIndex = -1;
}
Chunk::~Chunk()
{
	m_rParent.getSlaveLocator().free(m_slavePosition);
}

int Chunk::incDeaths()
{
	return m_deaths++;
}
Vec2 Chunk::getSpawn()
{
	return m_spawnPoint;
}
Vec2 Chunk::getClearSpawn()
{
	if(game.getUniverse().isClear(m_spawnPoint, getRadius(), this))
		return m_spawnPoint;
	else
		return game.getUniverse().getAvailableSpawn(m_body.getTeam(), getRadius(), this);
}
void Chunk::setSpawn(float x, float y)
{
	m_spawnPoint = Vec2(x, y);
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
List<sptr<Module>> Chunk::getModuleList() const
{
	return m_modules;
}
bool Chunk::allows(const Vec2& rGridPos)
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
		sptr<ModuleData> moduleDataCopy(rData.clone());
		moduleDataCopy->ioComp.pMyManager = &game.getUniverse().getUniverseIO();
		m_modules.push_back(sptr<Module>(moduleDataCopy->generate(m_body.getBodyPtr(), myPools, this)));
	}
	else
		cout << FILELINE;
}
void Chunk::prePhysUpdate()
{
	//push chunk in bounds if out of bounds
	Vec2 location = m_body.getBodyPtr()->GetPosition();
	Vec2 bounds = game.getUniverse().getBounds();
	if(abs(location.x) > bounds.x || abs(location.y) > bounds.y)
	{
		Vec2 force = Vec2(-location.x, -location.y);
		float diff = std::max(abs(location.x) - bounds.x, abs(location.y) - bounds.y);
		force = force.unit();
		force *= 10 * diff;
		m_body.getBodyPtr()->ApplyForceToCenter(force, true);
	}

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
const String& Chunk::getName() const
{
	return m_io.getName();
}
void Chunk::setAim(const Vec2& world)//send our aim coordinates
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
			String store;
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
			else
				assert(Print << "Store name [" + store + "]." << FILELINE);
			m_timer.restartCountDown();
		}
	}

	bool startThrusting = (rIssues[Directive::Up] && !m_wasThrusting);
	bool startBoosting = (rIssues[Directive::Up] && rIssues[Directive::Boost] && !m_wasBoosting);



	if(startThrusting)
	{
		for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
			(*it)->getAnimator().setAnimation("Activated", 0.20f);
		//Sound needed.
		//m_thrustNoiseIndex = game.getSound().playSound("Afterburner.wav", 100, 20, 2, getBodyPtr()->GetPosition(), true, true);
	}
	if(startBoosting)
	{
		for(auto it = afterburners_boost.begin(); it != afterburners_boost.end(); ++it)
			(*it)->getAnimator().setAnimation("Activated", 0.20f);
		//Sound needed.
		//m_boostNoiseIndex = game.getSound().playSound("Afterburner.wav", 100, 20, 2, getBodyPtr()->GetPosition(), true, true);
	}



	if(!rIssues[Directive::Up])
	{
		for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
			(*it)->getAnimator().setAnimation("Default", 1.0f);
		//Sound needed.
		//m_thrustNoiseIndex = game.getSound().stopSound(m_thrustNoiseIndex);
	}
	if(!rIssues[Directive::Up] || !rIssues[Directive::Boost])
	{
		for(auto it = afterburners_boost.begin(); it != afterburners_boost.end(); ++it)
			(*it)->getAnimator().setAnimation("Default", 1.0f);
		//Sound needed.
		//m_boostNoiseIndex = game.getSound().stopSound(m_boostNoiseIndex);
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
String Chunk::hasModuleAt(const Vec2 offset) const
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		if(offset == (*it)->getOffset())
			return (*it)->getTitle();
	return "";
}
/// <summary>
/// List of module BP names and 
/// </summary>
List<std::pair<String, Vec2> > Chunk::getModules() const
{
	List<std::pair<String, Vec2> > list;
	for(auto it = m_modules.cbegin(); it != m_modules.cend(); ++it)
	{
		if(dynamic_cast<ShipModule*>(it->get()) != NULL)//make sure it's not a strange item, like a ShieldComponent
		{
			//cout << "\nChunk: " << (*it)->getOffset().x << (*it)->getOffset().y;
			list.push_back(std::pair<String, Vec2>((*it)->getTitle(), Vec2((*it)->getOffset())));
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
void Chunk::input(String rCommand, sf::Packet rData)
{
	if(rCommand == "clear")
	{
		this->clear();
	}
	else if(rCommand == "attachModule")
	{
		String bpName;
		Vec2 offset;

		rData >> bpName;
		rData >> offset.x;
		rData >> offset.y;

		auto pNewModuleData = sptr<ModuleData>(m_rParent.getBlueprints().getModuleSPtr(bpName)->clone());
		if(pNewModuleData != NULL)
		{
			pNewModuleData->fixComp.offset = offset;
			this->add(*pNewModuleData);
		}
		else
		{
			cout << "\nBlueprint didn't exist." << FILELINE;
		}
	}
	else if(rCommand == "detachModule")
	{
		Vec2 targetOffset;//the offset we are looking to remove

		rData >> targetOffset.x;
		rData >> targetOffset.y;

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
			cout << "\nThere was no module at " << targetOffset.x << "," << targetOffset.y << " " << FILELINE;
	}
	else
		cout << "\nCommand not found in [" << m_io.getName() << "]." << FILELINE;
}
float Chunk::getRadius()
{
	if(m_radius > 0.f)
		return m_radius;
	Vec2 max(0,0);
	for(auto it = m_modules.cbegin(); it != m_modules.cend(); ++it)
	{
		if(max.len() < Vec2((*it)->getOffset()).len())
		{
			max = Vec2((*it)->getOffset());
		}
	}
	m_radius = max.len();
	return m_radius;
}

