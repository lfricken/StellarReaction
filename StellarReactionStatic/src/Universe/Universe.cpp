#include "Universe.hpp"
#include "BlueprintLoader.hpp"
#include "Globals.hpp"
#include "SlaveLocator.hpp"
#include "BatchLayers.hpp"
#include "GraphicsComponentUpdater.hpp"
#include "IOManager.hpp"
#include "Player.hpp"
#include "QuadComponent.hpp"
#include "Chunk.hpp"
#include "Chunk.hpp"
#include "ShipModule.hpp"
#include "Sensor.hpp"
#include "Thruster.hpp"
#include "Reactor.hpp"
#include "Capacitor.hpp"
#include "Turret.hpp"
#include "LaserWeapon.hpp"
#include "LinearMeter.hpp"
#include "BallisticWeapon.hpp"
#include "ProjectileMan.hpp"
#include "Convert.hpp"
#include "DecorationEngine.hpp"
#include "CaptureArea.hpp"
#include "UpgradeType.hpp"
#include "Overlay.hpp"
#include "Plating.hpp"



void Universe::loadLevel(const GameLaunchData& data)//loads a level using blueprints
{

	for(int i = (int)Team::MinTeam + 1; i < (int)Team::MaxTeam; ++i) // initialize team resources
	{
		m_teamResources[(Team)i] = Resources();
		m_teamResources[(Team)i].m_resourceValues[Resources::Matter] = 30;
	}

	ShipBuilder::resetSlaveName();
	m_spControlFactory.reset(new ControlFactory);//remove all controllers.

	loadBlueprints("blueprints/");

	const String levelFile = "level.lcfg";
	const String levelFolder = "levels";
	const String thisLevelFolder = contentDir() + levelFolder + "/" + data.level + "/";
	const String modDir = "mods/";

	std::ifstream level(thisLevelFolder + levelFile, std::ifstream::binary);
	Json::Reader reader;
	Json::Value root;

	bool parsedSuccess = reader.parse(level, root, false);

	if(!parsedSuccess)
	{
		Print << "\nParse Failed [" << thisLevelFolder + levelFile << "].\n" << FILELINE;
		return;
	}
	else
	{
		/**Additional Map Blueprints**/
		if(!root["AdditionalBlueprints"].isNull())
		{
			const Json::Value bpList = root["AdditionalBlueprints"];
			for(auto it = bpList.begin(); it != bpList.end(); ++it)
			{
				m_spBPLoader->loadBlueprints(thisLevelFolder + it->asString());
			}
		}
		/**Map Bounds*/
		GETJSON(m_bounds);
		GETJSON(m_lanes);

		if(!root["BuildBounds"].isNull())
		{
			String ChunkName;
			const Json::Value typeList = root["BuildBounds"];
			for(auto it = typeList.begin(); it != typeList.end(); ++it)
			{
				String ChunkName = (*it)[NAMEOF(ChunkName)].asString();
				const Json::Value boundList = (*it)["BoundList"];
				for(auto bound = boundList.begin(); bound != boundList.end(); ++bound)
				{
					BuildBounds bounds;
					bounds.center.x = (*bound)["center"][0].asFloat();
					bounds.center.y = (*bound)["center"][1].asFloat();
					bounds.radius = (*bound)["radius"].asFloat();
					m_buildBounds[ChunkName].push_back(bounds);
				}
			}
		}

		/**Spawn Points**/
		if(!root["SpawnPoints"].isNull())
		{
			const Json::Value spawnList = root["SpawnPoints"];
			for(auto it = spawnList.begin(); it != spawnList.end(); ++it)
			{
				const Json::Value pointsJson = (*it)["Points"];
				List<Vec2> points;
				for(auto itPoint = pointsJson.begin(); itPoint != pointsJson.end(); ++itPoint)
				{
					points.push_back(Vec2((*itPoint)[0].asFloat(), (*itPoint)[1].asFloat()));
				}
				m_spawnPoints[(Team)(*it)["Team"].asInt()] = points;
			}
		}

		/**Create player ships**/
		for(auto it = data.playerList.cbegin(); it != data.playerList.cend(); ++it)
		{
			int num = it - data.playerList.cbegin();
			Vec2 spawn = m_spawnPoints[it->team][num];
			float angle = Math::toDeg(atan2(spawn.y, spawn.x) + (Math::Pi / 2.f));

			ChunkDataMessage chunkMessageData;

			chunkMessageData.blueprintName = it->ship;
			chunkMessageData.coordinates = spawn;
			chunkMessageData.rotation = angle;
			chunkMessageData.team = (int)it->team;
			chunkMessageData.needsController = true;
			chunkMessageData.aiControlled = false;

			ShipBuilder::Server::createChunk(chunkMessageData, 0);
		}

		/**Load Local Player Overlay**/
		getGame()->getLocalPlayer().loadOverlay("overlayconfig");

		/**Set Local Controller**/
		getGame()->getLocalPlayer().setController(data.localController);

		/**Hazard Field Spawn Hazards**/
		//for(auto it = hazardFields.begin(); it != hazardFields.end(); ++it)
		//	(**it).spawn();

		/**Map Chunks**/
		if(!root["Chunks"].isNull())
		{
			const Json::Value chunks = root["Chunks"];
			for(auto it = chunks.begin(); it != chunks.end(); ++it)
			{
				ChunkDataMessage chunkMessageData;
				chunkMessageData.loadJson(*it);

				ShipBuilder::Server::createChunk(chunkMessageData, 0);
			}
		}
		const String Custom = "Custom";
		if(!root[Custom].isNull())
		{
			const String fixtureBpNameRoot = Custom;
			const String chunkBpNameRoot = Custom;
			const Json::Value custom = root[Custom];

			int index = 0;
			for(auto it = custom.begin(); it != custom.end(); ++it)
			{
				const String fixtureBpName = fixtureBpNameRoot + String(index);
				const String chunkBpName = chunkBpNameRoot + String(index);

				auto fixtureBp = sptr<PlatingData>(static_cast<PlatingData*>(getBlueprints().getModuleSPtr(fixtureBpNameRoot)->clone()));
				auto chunkBp = sptr<ChunkData>(static_cast<ChunkData*>(getBlueprints().getChunkSPtr(chunkBpNameRoot)->clone()));

				fixtureBp->fixComp.vertices = JSON::get(*it, "vertices", fixtureBp->fixComp.vertices);
				chunkBp->moduleData.push_back(Pair<String, Vec2>(fixtureBpName, Vec2(0, 0)));

				getBlueprints().addBp(fixtureBpName, fixtureBp);
				getBlueprints().addBp(chunkBpName, chunkBp);

				ChunkDataMessage chunkMessageData;
				chunkMessageData.blueprintName = chunkBpName;
				ShipBuilder::Server::createChunk(chunkMessageData, 0);

				++index;
			}
		}
		/**Hazard Fields**/
		if(!root["Spawners"].isNull())
		{
			const Json::Value spawnList = root["Spawners"];
			for(auto it = spawnList.begin(); it != spawnList.end(); ++it)
				hazardFields.push_back(sptr<ChunkSpawner>(new ChunkSpawner(this, *it)));
		}
		/**Decorations**/
		DecorationEngine& decorations = *m_spDecorEngine;
		LOADJSON(decorations);
	}

	Message initBackground(this->m_io.getName(), "initBackgroundCommand", voidPacket, 0, false);
	Message::SendUniverse(initBackground);

	getGame()->getOverlay().resetStore();
	getGame()->getOverlay().addStoreButton();
	getGame()->getOverlay().addStoreButton();
	getGame()->getOverlay().addStoreButton();
}
void Universe::createControllers(Team team, bool isAnAI, const String& slaveName, int& controller, int& aiPos)
{
	controller = m_spControlFactory->addController(slaveName);

	if(isAnAI && !getGame()->getNwBoss().isClient())
	{
		sptr<ShipAI> ai(new ShipAI(team, controller));
		m_shipAI.insert(ai);
		aiPos = ai->getFactoryPosition();
	}
}
Universe::Universe(const IOComponentData& rData) : m_io(rData, &Universe::input, this), m_physWorld(Vec2(0, 0))
{
	m_allModulesUnlocked = false;
	const Money defaultTickMoney = 1;
	const float moneyTickTime = 5.f; // How often do we get income applied?

	m_velocityIterations = 1;
	m_positionIterations = 1;
	m_timeStep = 1.0f / 120.0f; // TODO LOAD FROM FILE

	m_nw.reset(new NetworkComponent(NetworkComponentData(), &Universe::pack, &Universe::unpack, this, getGame()->getNwBoss().getNWFactoryTcp()));

	m_spBPLoader = sptr<BlueprintLoader>(new BlueprintLoader);
	m_spBatchLayers = sptr<BatchLayers>(new BatchLayers);
	m_spGfxUpdater = sptr<GraphicsComponentUpdater>(new GraphicsComponentUpdater);
	m_spControlFactory = sptr<ControlFactory>(new ControlFactory);

	/**IO**/
	m_spUniverseIO = sptr<IOManager>(new IOManager(true, true));
	m_spUniverseIO->give(&m_io);
	m_spUniverseIO->give(&getGame()->getLocalPlayer().getIOComp());
	/**IO**/


	//how often are people given capture rewards?
	m_spDecorEngine.reset(new DecorationEngine);
	m_spMoneyTimer.reset(new Timer(this->getTime()));
	m_spMoneyTimer->setCountDown(moneyTickTime);
	m_restartedMoneyTimer = false;

	/**PHYControlCS**/
	m_paused = false;
	m_skippedTime = getGame()->getTime();
	m_pauseTime = m_skippedTime;

	m_inc = 10;
	m_currentBed = Vec2(-10000, 10000);
	m_bounds = Vec2(10000, 10000);

	m_physWorld.SetContactListener(&m_contactListener);
	m_physWorld.SetDebugDraw(&m_debugDraw);
	m_debugDraw.SetFlags(b2Draw::e_shapeBit);
	/**PHYControlCS**/

	m_debugDrawEnabled = false;

	setTime(getGame()->getTime());

}
void Universe::postConstructor()
{
	m_spProjMan = sptr<ProjectileMan>(new ProjectileMan); // because it needs to do IO
}
Universe::~Universe()
{
	getGame()->getLocalPlayer().onBeforeUniverseDestroyed();
}
int Universe::getChunkPosition(String& name)
{
	for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		sptr<Chunk> chunk = *it;;
		if(chunk.get() != nullptr)
			if(chunk->m_io.getName() == name)
				return (it - m_goList.begin());
	}
	return -1;
}
Chunk* Universe::getChunk(int pos)
{
	return m_goList.get(pos);
}
ControlFactory& Universe::getControllerFactory()
{
	return *m_spControlFactory;
}
ProjectileMan& Universe::getProjMan()
{
	return *m_spProjMan;
}
DecorationEngine& Universe::getDecors()
{
	return *m_spDecorEngine;
}
//SlaveLocator& Universe::getSlaveLocator()
//{
//	return *m_spSlaveLocator;
//}
b2World& Universe::getWorld()
{
	return m_physWorld;
}
BlueprintLoader& Universe::getBlueprints()
{
	return *m_spBPLoader;
}
BatchLayers& Universe::getBatchLayers()
{
	return *m_spBatchLayers;
}
GraphicsComponentUpdater& Universe::getGfxUpdater()
{
	return *m_spGfxUpdater;
}
Factory<ShipAI>& Universe::getShipAI()
{
	return m_shipAI;
}
Factory<Chunk>& Universe::getChunks()
{
	return m_goList;
}
IOManager& Universe::getUniverseIO()
{
	return *m_spUniverseIO;
}
Scoreboard& Universe::getScoreboard()
{
	return *m_scoreboard;
}
const Vec2& Universe::getBounds() const
{
	return m_bounds;
}
void Universe::setBounds(const Vec2& bounds)
{
	m_bounds = bounds;
}
void Universe::updateShipAI()
{
	for(auto it = m_shipAI.begin(); it != m_shipAI.end(); ++it)
	{
		sptr<ShipAI> chunk = *it;
		if(chunk.get() != nullptr)
			chunk->updateDecision();
	}
}
/// <summary>
/// If true, we only draw box2d things on screen.
/// </summary>
void Universe::toggleDebugDraw()
{
	m_debugDrawEnabled = !m_debugDrawEnabled;
}
/// <summary>
/// get the time step for the box2d::world
/// </summary>
float Universe::getTimeStep() const
{
	return m_timeStep;
}
/// <summary>
/// Where we call prePhysUpdate on all Chunks
/// </summary>
void Universe::prePhysUpdate()
{
	if(!m_paused)
	{
		for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			sptr<Chunk> chunk = *it;;
			if(chunk.get() != nullptr)
				chunk->prePhysUpdate();
		}
		m_spProjMan->prePhysUpdate();
	}
}
void Universe::changeTeamResources(Team team, Resources money)
{
	this->m_teamResources[team].add(money);
}
void Universe::changeTeamIncome(Team team, Resources money)
{
	this->m_teamIncome[team].add(money);
}
void Universe::physUpdate()
{
	if(!m_paused)
	{
		m_physWorld.Step(m_timeStep, m_velocityIterations, m_positionIterations);
	}
}
/// <summary>
/// Where we call postPhysUpdate on all Chunks
/// </summary>
void Universe::postPhysUpdate()
{
	if(!m_paused)
	{
		for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			sptr<Chunk> chunk = *it;;
			if(chunk.get() != nullptr)
				chunk->postPhysUpdate();
		}

		m_spProjMan->postPhysUpdate();

		for(auto it = hazardFields.begin(); it != hazardFields.end(); ++it)
		{
			sptr<ChunkSpawner> spawner = *it;
			if(spawner.get() != nullptr)
				spawner->update();
		}
	}
}

Chunk* Universe::getNearestChunk(const Vec2& target, const ModuleParent* exception, std::list<Team> validTeams)
{
	float prevDist = -1;
	Chunk* closest = nullptr;
	for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		Chunk* chunk = it->get();;
		if(chunk != nullptr && chunk != exception && listContains(validTeams, chunk->getBodyComponent().getTeam()))
		{
			Vec2 dif = target - chunk->getBodyComponent().getPosition();
			float dist = dif.len();
			if(dist < prevDist || prevDist == -1)
			{
				prevDist = dist;
				closest = chunk;
			}
		}
	}
	return closest;
}
/// <summary>
/// returns true if list contains value or list is empty
/// </summary>
bool Universe::listContains(std::list<Team> intList, Team value)
{
	if(intList.empty())
		return true;

	for(auto it = intList.begin(); it != intList.end(); ++it)
	{
		if((*it) == value)
			return true;
	}
	return false;
}
/// <summary>
/// Gives each team the money the get per step (from cap points)
/// </summary>
void Universe::teamMoneyUpdate()
{
	if(!m_restartedMoneyTimer) // TODO why is this needed?
	{
		m_spMoneyTimer->restartCountDown();
		m_restartedMoneyTimer = true;
	}

	if(!getGame()->getNwBoss().isClient()) // skip a bit of work
		if(m_spMoneyTimer->isTimeUp())
		{
			for(auto it = m_teamIncome.cbegin(); it != m_teamIncome.cend(); ++it)
				Resources::ChangeResources(it->second, it->first);

			m_spMoneyTimer->restartCountDown();
		}
}
/// <summary>
/// returns true if debug draw is on
/// debug draw is drawing box2d shapes only
/// </summary>
/// <returns></returns>
bool Universe::debugDraw() const
{
	return m_debugDrawEnabled;
}
/// <summary>
/// Toggles pause on the Universe
/// this affects Timer
/// </summary>
void Universe::togglePause(bool pause)
{
	if(m_paused && !pause)//switched to not paused
		m_skippedTime += m_realTime - m_pauseTime;
	else if(!m_paused && pause)//switch to paused
		m_pauseTime = m_realTime;

	m_paused = pause;
}
/// <summary>
/// Toggles pause on the Universe
/// this affects Timer
/// </summary>
void Universe::togglePause()
{
	togglePause(!m_paused);
}
bool Universe::isPaused()
{
	return m_paused;
}
/// <summary>
/// Gets universe time
/// this can be paused and resumed
/// </summary>
float Universe::getTime() const
{
	float universeTime;
	if(m_paused)
		universeTime = m_pauseTime - m_skippedTime;
	else
		universeTime = m_realTime - m_skippedTime;

	return universeTime;
}
void Universe::setTime(float time)
{
	m_realTime = time;
}
Vec2 Universe::getBed()//give a position to sleep at
{
	Vec2 bed;
	if(m_beds.size() > 0)//if we have some already existing
	{
		bed = m_beds.back();
		m_beds.pop_back();
	}
	else
	{
		m_currentBed.x += m_inc;
		bed = m_currentBed;
	}
	return bed;
}
void Universe::addBed(const Vec2& rBed)//someone gave a bed back to us!
{
	m_beds.push_back(rBed);
}
void Universe::loadBlueprints(const String& bpDir)//loads blueprints
{
	m_spBPLoader->loadBlueprints(bpDir);
}
int Universe::add(sptr<Chunk> chunk)
{
	m_goList.insert(chunk);
	return chunk->getFactoryPosition();
}
const Resources& Universe::getTeamResources(Team team) const
{
	const auto it = m_teamResources.find(team);
	return it->second;
}
bool Universe::canBuildAtLocation(String chunkBPName, Vec2 pos) const
{
	auto it = m_buildBounds.find(chunkBPName);
	if(it != m_buildBounds.cend())
	{
		const auto& list = it->second;
		for each (auto bounds in list)
		{
			if(bounds.contains(pos))
				return true;
		}
		return false;
	}
	Print << "\n\"" << chunkBPName << "\" has no build positions on this map!";
	return false;
}
Vec2 Universe::getLaneTarget(Team team, Lane lane, const Vec2& pos) const
{
	int deltaDirection;
	if(team == Team::One)
		deltaDirection = 1;
	else
		deltaDirection = -1;

	auto& points = m_lanes[(int)lane];
	int index = 0;
	int i = 0;
	auto dist = pos.to(points[index]).len();
	for each (auto point in points)
	{
		auto distToPoint = pos.to(point).len();
		if(distToPoint < dist)
		{
			index = i;
			dist = distToPoint;
		}
		++i;
	}
	index += deltaDirection;
	//modify target index
	if(index < 0)
		index = 0;
	else if(index >= points.size())
		index = points.size() - 1;

	return points[index];
}
void Universe::pack(sf::Packet& data)
{
	//TODO sync money maybe? Looks like that's already being done via seperate message
}
void Universe::unpack(sf::Packet& data)
{

}
void Universe::input(String rCommand, sf::Packet rData)
{
	sf::Packet data(rData);
	if(rCommand == "togglePause")
	{
		togglePause(!m_paused);
	}
	else if(rCommand == "setPause")
	{
		bool mode;
		data >> mode;
		togglePause(mode);
	}
	else if(rCommand == "initBackgroundCommand")
	{
		auto controller = getGame()->getLocalPlayer().getController();
		if(controller != nullptr)
		{
			auto pos = controller->getChunk()->getBodyComponent().getPosition();
			float maxZoom = getGame()->getLocalPlayer().getCamera().m_maxZoom * 0.4f;
			float sizeInUniverse = Convert::screenToUniverse((float)getGame()->getWindow().getSize().x);
			m_spDecorEngine->initSpawns(pos, Vec2(maxZoom * sizeInUniverse, maxZoom * sizeInUniverse));
		}

		getGame()->getUniverse().getControllerFactory().setAllNonLocallyControlled();
	}
	else if(rCommand == "createChunkCommand")
	{
		String slaveName;
		ChunkDataMessage data;
		data.unpack(rData);

		if(data.needsController)
			slaveName = ShipBuilder::getNextSlaveName();


		auto chunkData(m_spBPLoader->getChunkSPtr(data.blueprintName)->clone());
		chunkData->bodyComp.coords = data.coordinates;
		chunkData->bodyComp.rotation = data.rotation;
		chunkData->bodyComp.team = (Team)data.team;
		chunkData->ioComp.name = slaveName;


		//important this happens before creation of the controller
		sptr<Chunk> chunk = chunkData->generate(this);
		int chunkIndex = add(chunk);

		int controller = -1;
		int ai = -1;
		if(data.needsController)
			createControllers((Team)data.team, data.aiControlled, slaveName, controller, ai);// data.slaveName;

		chunk->m_controller = controller;
		chunk->m_shipAI = ai;
	}
	else if(rCommand == "killChunkCommand")
	{
		int chunkListPosition;
		bool shouldShakeScreen;

		rData >> chunkListPosition;
		rData >> shouldShakeScreen;

		if(chunkListPosition < m_goList.size())
		{
			Print << "\nkill " << chunkListPosition;

			auto chunk = m_goList[chunkListPosition];

			if(chunk != nullptr)
			{ // perception increase for enemy dying
				sf::Packet data;
				int team;
				if(chunk->getBodyComponent().getTeam() == Team::One)
					team = (int)Team::Two;
				else
				{
					team = (int)Team::One;
				}

				data << team;

				Resources perceptionIncrease;
				perceptionIncrease.m_resourceValues[Resources::Perception] = 1;
				perceptionIncrease.intoPacket(&data);

				Message killPerception("universe", "changeResources", data, 0.f, false);
				Message::SendUniverse(killPerception);
			}
			else
				WARNING;

			m_goList.freeThis(chunkListPosition);
			if(shouldShakeScreen)
				getGame()->getLocalPlayer().getCamera().shake(0.5, 60, 0.5);
		}
		else
			WARNING;
	}
	else if(rCommand == "upgrade")
	{
		Print << "\n" << rCommand;

		int teamT;
		rData >> teamT;
		Team team = (Team)teamT; // Which team got the perception.

		//float unlockOdds = 99.f / 100.f;
		//float roll = Rand::get(0.f, 1.f);
		if(!m_allModulesUnlocked)// && (roll < unlockOdds))
		{
			m_allModulesUnlocked = getGame()->getOverlay().addStoreButton();
		}
		else
		{
			String blueprint = chooseBPtoUpgrade();
			UpgradeType upgradeType = chooseUpgradeType();

			int count = m_spBPLoader->upgrade(blueprint, upgradeType, team);
			Print << "\n" << blueprint << " " << count; // << " " << (int)upgradeType;
		}
	}
	else if(rCommand == "changeIncome")
	{
		int team;
		Resources delta;
		rData >> team;
		delta.fromPacket(&rData);

		changeTeamIncome((Team)team, delta);
	}
	else if(rCommand == "changeResources")
	{
		int team;

		Resources delta;
		rData >> team;
		delta.fromPacket(&rData);

		changeTeamResources((Team)team, delta);
	}
	else
	{
		Print << "\n" << m_io.getName() << ":[" << rCommand << "] not found." << FILELINE;
	}
}
UpgradeType Universe::chooseUpgradeType()
{
	int max = (int)UpgradeType::Other;
	UpgradeType gen = (UpgradeType)Rand::get(0, max); // TODO, this api needs to update to be exclusive, and then the code needs to be fixed.

	return gen;
}
String Universe::chooseBPtoUpgrade()
{
	auto& data = getGame()->getOverlay().storeData;
	List<String> blueprints; // possible blueprints to choose from

	for each(auto button in data->buttonList)
	{
		blueprints.push_back(button.moduleBlueprint);
	}
	return blueprints[Rand::get(0, blueprints.size() - 1)];
}
bool Universe::isClear(const Vec2& position, float radius, const ModuleParent* exception)
{
	auto nearestChunk = getNearestChunk(position, exception);
	float nearestRad = nearestChunk->getRadius();
	float dist = (nearestChunk->getBodyComponent().getPosition() - position).len();

	if(dist < (nearestRad + radius))
		return false;
	else
		return true;
}
Vec2 Universe::getAvailableSpawn(Team team, float radius, const ModuleParent* exception)
{
	for(auto it = m_spawnPoints[team].begin(); it != m_spawnPoints[team].end(); it++)
	{
		if(isClear((*it), radius, exception))
			return (*it);
	}
	return Vec2(0, 0);
}
void Universe::spawnParticles(const String& particleBP, const Vec2& pos, const Vec2& dir, const Vec2& transverse)
{
	m_spDecorEngine->spawnParticles(particleBP, pos, dir, transverse);
}
