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
#include "Shield.hpp"
#include "Grid.hpp"
#include "Resources.hpp"


void ChunkDataMessage::loadJson(const Json::Value& root)
{
	GETJSON(blueprintName);
	GETJSON(coordinates);
	GETJSON(rotation);
	GETJSON(team);
	GETJSON(needsController);
	GETJSON(aiControlled);

	if(aiControlled)
		needsController = true;
}
void ChunkDataMessage::pack(sf::Packet* data) const
{
	sf::Packet& pack = *data;
	pack << blueprintName;
	pack << coordinates.x << coordinates.y;
	pack << rotation;
	pack << team;
	pack << slaveName;
	pack << needsController;
	pack << aiControlled;
}
void ChunkDataMessage::unpack(const sf::Packet& data)
{
	sf::Packet pack = data;
	pack >> blueprintName;
	pack >> coordinates.x >> coordinates.y;
	pack >> rotation;
	pack >> team;
	pack >> slaveName;
	pack >> needsController;
	pack >> aiControlled;
}
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
	GETJSON(minShieldPower);

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
				auto bp = game.getUniverse().getBlueprints().getModuleSPtr(title);
				auto clone = bp->clone();
				spMod.reset(clone);

				spMod->fixComp.offset.x = (*it)["Position"][0].asFloat();
				spMod->fixComp.offset.y = (*it)["Position"][1].asFloat();
			}
			else
			{
				Print << "\n" << FILELINE;
				///ERROR LOG
			}

			moduleData.push_back(spMod);
		}
	}
}


List<Vec2> taxicabCircle(int radius)
{
	List<Vec2> coordinates;

	for(int i = 1; i < radius; ++i)//skip the 4 corner coordinates (otherwise they get repeated
	{
		coordinates.push_back(Vec2((float)i, (float)radius - i));
	}

	{//copy, mirror across y axis, and add.
		auto topLeft(coordinates);
		for(auto it = topLeft.begin(); it != topLeft.end(); ++it)
		{
			it->x = -it->x;
		}
		coordinates.insert(coordinates.end(), topLeft.begin(), topLeft.end());
	}
	{//copy, mirror across x axis, and add.
		auto bottom(coordinates);
		for(auto it = bottom.begin(); it != bottom.end(); ++it)
		{
			it->y = -it->y;
		}
		coordinates.insert(coordinates.end(), bottom.begin(), bottom.end());
	}
	{//finally add the last 4 coordinates
		coordinates.push_back(Vec2((float)radius, 0.f));
		coordinates.push_back(Vec2((float)-radius, 0.f));
		coordinates.push_back(Vec2(0.f, (float)radius));
		coordinates.push_back(Vec2(0.f, (float)-radius));
	}
	return coordinates;
}


Chunk::Chunk(const ChunkData& rData) : GameObject(rData), m_body(rData.bodyComp), m_zoomPool(rData.zoomData), m_energyPool(rData.energyData), m_ballisticPool(rData.ballisticData), m_missilePool(rData.missileData)
{
	m_body.parent = this;

	m_resources.reset(new Resources);
	m_resources->m_resourceValues.begin()->second = 139;
	m_inDeathProcess = false;
	m_canDie = true;
	m_shieldToggleTimer.setCountDown(0.5f);
	m_shieldToggleTimer.restartCountDown();

	m_body.setTeam(rData.team);//important that team is set before module creation
	m_spawnPoint = m_body.getBodyPtr()->GetPosition();
	m_radius = -1.f;
	m_deaths = 0;
	m_wasThrusting = false;
	m_wasBoosting = false;
	m_stealth = false;
	m_timer.setCountDown(1);
	m_timer.restartCountDown();
	m_areShieldsOn = false;
	m_minShieldPower = rData.minShieldPower;

	//Set valid module positions.
	m_validOffsets = rData.validPos;

	//Add Modules.
	for(auto it = rData.moduleData.cbegin(); it != rData.moduleData.cend(); ++it)
		add(**it);

	//Register for controlling.


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
	//m_rParent.getSlaveLocator().free(m_slavePosition);
	//Print << "\n dead chunk.";
	if(m_controller != -1)//this ship might not have a controller or ai
		m_rParent.getControllerFactory().m_list.free(m_controller);
	if(m_shipAI != -1)
		m_rParent.getShipAI().free(m_shipAI);
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
		auto module = sptr<Module>(moduleDataCopy->generate(m_body.getBodyPtr(), myPools, this));
		m_modules.push_back(module);
	}
	else
		Print << FILELINE;
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
	m_lastAim = world;
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->setAim(m_lastAim);
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
			//m_lastAim;
			//String store;
			//for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
			//{
			//	store = (*it)->getStore();
			//	if(store != "")
			//		break;
			//}
			//if(store != "")
			//{
			//	Message toggle(store, "toggleHidden", voidPacket, 0, false);
			//	Message mes2("local_player", "toggleGuiMode", voidPacket, 0, false);
			//	game.getCoreIO().recieve(toggle);
			//	game.getCoreIO().recieve(mes2);
			//}
			//else
			//	assert(Print << "Store name [" + store + "]." << FILELINE);

			String store = "store_default";

			Message toggle(store, "toggleHidden", voidPacket, 0, false);
			Message mes2("local_player", "toggleGuiMode", voidPacket, 0, false);
			game.getCoreIO().recieve(toggle);
			game.getCoreIO().recieve(mes2);

			m_timer.restartCountDown();
		}
	}

	bool startThrusting = (rIssues[Directive::Up] && !m_wasThrusting);
	bool startBoosting = (rIssues[Directive::Up] && rIssues[Directive::Boost] && !m_wasBoosting);


	//Shields
	if(rIssues[Directive::Shield] && m_shieldToggleTimer.isTimeUp())
	{
		m_shieldToggleTimer.restartCountDown();
		Message shield;

		bool enoughEnergy = m_energyPool.getPercent() > 0.25f;
		if(!m_areShieldsOn && enoughEnergy)//if they aren't already on and we have enough energy turn them on
			shield.reset(m_io.getPosition(), "enableShields", voidPacket, 0, false);
		else if(m_areShieldsOn)//if they are on turn them off no mattter what.
			shield.reset(m_io.getPosition(), "disableShields", voidPacket, 0, false);

		Message::SendUniverse(shield);
	}


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

	case(Request::ShieldState) :
		return static_cast<float>(m_areShieldsOn);


	default:
		return 0.f;
		break;
	}
}
/// <summary>
/// returns the title of the module at that position, otherwise ""
/// </summary>
String Chunk::hasModuleAt(const sf::Vector2i offset) const
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
	{
		Vec2 pos = (*it)->getOffset();
		if(offset == sf::Vector2i((int)pos.x, (int)pos.y))
			return (*it)->getTitle();

	}
	return "";
}
/// <summary>
/// List of module BP names and 
/// </summary>
List<std::pair<String, sf::Vector2i> > Chunk::getModules() const
{
	List<std::pair<String, sf::Vector2i> > list;
	for(auto it = m_modules.cbegin(); it != m_modules.cend(); ++it)
	{
		if(dynamic_cast<ShipModule*>(it->get()) != nullptr)//make sure it's not a strange item, like a ShieldComponent
		{
			//Print << "\nChunk: " << (*it)->getOffset().x << (*it)->getOffset().y;
			Vec2 pos = (*it)->getOffset();
			list.push_back(std::pair<String, sf::Vector2i>((*it)->getTitle(), sf::Vector2i((int)pos.x, (int)pos.y)));
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

ShipModule* Chunk::getNearestValidTarget(Vec2 target)
{
	Map<Vec2, ShipModule*> moduleCoordinates;
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
	{
		ShipModule* shipModule = dynamic_cast<ShipModule*>(it->get());
		if(shipModule != nullptr && shipModule->getHealth().getHealthPercent() > 0.f)
		{
			Vec2 pos = shipModule->getFixtureComponent().getOffset() - target;
			moduleCoordinates[pos] = shipModule; // -target because we need to offset damage wave origin
		}
	}


	List<ShipModule*> availableTargets;
	bool hits = false;
	for(int i = 1; hits == false && i < 4; ++i)
	{
		List<Vec2> circle = taxicabCircle(i);

		for(auto cirPos = circle.cbegin(); cirPos != circle.cend(); ++cirPos)
		{
			for(auto modulePos = moduleCoordinates.cbegin(); modulePos != moduleCoordinates.cend(); ++modulePos)
			{
				if((*cirPos) == (modulePos->first))
				{
					hits = true;
					availableTargets.push_back(modulePos->second);
				}
			}
		}
	}

	if(availableTargets.size() == 0)//the chunk is dead
	{
		dout << "\nDead.";

		if(m_canDie && !m_inDeathProcess)
		{
			m_inDeathProcess = true;

			game.getUniverse().spawnParticles("SparkExplosion", getBodyComponent().getPosition(), Vec2(1, 0), Vec2(0, 0));
			game.getUniverse().spawnParticles("WhiteFlash", getBodyComponent().getPosition(), Vec2(1, 0), Vec2(0, 0));
			game.getUniverse().spawnParticles("DebrisExplosion1", getBodyComponent().getPosition(), Vec2(1, 0), Vec2(0, 0));
			game.getUniverse().spawnParticles("DebrisExplosion2", getBodyComponent().getPosition(), Vec2(1, 0), Vec2(0, 0));
			game.getUniverse().spawnParticles("DebrisExplosion3", getBodyComponent().getPosition(), Vec2(1, 0), Vec2(0, 0));

			destroy(universePosition, true, 0.1f);

			ChunkDataMessage ship;

			ship.aiControlled = false;
			ship.needsController = false;
			ship.coordinates = getBodyComponent().getPosition();
			ship.blueprintName = "Loot";
			ship.rotation = 0;
			ship.team = static_cast<int>(Team::Neutral);
			ShipBuilder::Client::createChunk(ship, 0.3f);
		}
		//TODO death effects

		return nullptr;
	}

	int choice = Rand::get(0, availableTargets.size() - 1);//it's inclusive for ints!
	return availableTargets[choice];
}
void Chunk::destroy(int targetChunkUniversePos, bool shake, float delay)
{
	sf::Packet myPosData;
	myPosData << targetChunkUniversePos;
	myPosData << shake;
	Message death("universe", "killChunkCommand", myPosData, delay, false);
	Message::SendUniverse(death);
}
float Chunk::getRadius()
{
	if(m_radius > 0.f)
		return m_radius;
	Vec2 max(0, 0);
	for(auto it = m_modules.cbegin(); it != m_modules.cend(); ++it)
		if(max.len() < Vec2((*it)->getOffset()).len())
			max = Vec2((*it)->getOffset());

	m_radius = max.len();
	return m_radius;
}
void Chunk::resetStatusBoard(wptr<leon::Grid> grid)
{
	m_statusBoard = grid;
	if(auto board = m_statusBoard.lock())
	{
		board->reset(getModules());
		auto modules = getModuleList();
		for(auto it = modules.begin(); it != modules.end(); ++it)
		{
			auto module = dynamic_cast<ShipModule*>(it->get());
			if(module != nullptr)
			{
				board->damageModule(module->getOffset(), module->getHealthState(), module->getHealth().getHealthPercent(), false);
			}
		}
	}//else we set it to null
}
wptr<leon::Grid> Chunk::getStatusBoard()
{
	return m_statusBoard;
}

void Chunk::input(String rCommand, sf::Packet rData)
{
	if(rCommand == "clear")
	{
		this->clear();
	}
	else if(rCommand == "pickupLoot")
	{
		Resources loot;
		int lootChunkUniversePos;

		loot.outOf(&rData);
		rData >> lootChunkUniversePos;

		m_resources->add(loot);
		destroy(lootChunkUniversePos, false, 0.f);
	}
	else if(rCommand == "rebuiltComplete")
	{
		resetStatusBoard(m_statusBoard);
	}
	else if(rCommand == "doneBuilding")
	{
		if(auto grid = m_statusBoard.lock())
		{
			grid->reset(getModules());
		}
	}
	else if(rCommand == "attachModule")
	{
		String bpName;
		sf::Vector2i pos;

		rData >> bpName;
		rData >> pos.x;
		rData >> pos.y;

		auto pNewModuleData = sptr<ModuleData>(m_rParent.getBlueprints().getModuleSPtr(bpName)->clone());
		if(pNewModuleData != NULL)
		{
			pNewModuleData->fixComp.offset = Vec2((float)pos.x, (float)pos.y);
			this->add(*pNewModuleData);
		}
		else
		{
			Print << "\nBlueprint didn't exist." << FILELINE;
		}
	}
	else if(rCommand == "detachModule")
	{
		sf::Vector2i pos;//the offset we are looking to remove

		rData >> pos.x;
		rData >> pos.y;

		Vec2 targetOffset((float)pos.x, (float)pos.y);

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
			Print << "\nThere was no module at " << targetOffset.x << "," << targetOffset.y << " " << FILELINE;
	}
	else if(rCommand == "enableShields")
	{
		m_areShieldsOn = true;
		for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		{
			Shield* pShield = dynamic_cast<Shield*>(it->get());
			if(pShield)
				pShield->enableShield();
		}
	}
	else if(rCommand == "disableShields")
	{
		m_areShieldsOn = false;
		for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		{
			Shield* pShield = dynamic_cast<Shield*>(it->get());
			if(pShield)
				pShield->disableShield();
		}
	}
	else
		Print << "\nCommand not found in [" << m_io.getName() << "]." << FILELINE;
}



