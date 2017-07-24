#include "Universe.hpp"
#include "BlueprintLoader.hpp"
#include "Globals.hpp"
#include "SlaveLocator.hpp"
#include "BatchLayers.hpp"
#include "GraphicsComponentUpdater.hpp"
#include "IOManager.hpp"
#include "Player.hpp"
#include "QuadComponent.hpp"
#include "GameObject.hpp"
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



void Universe::loadLevel(const GameLaunchData& data)//loads a level using blueprints
{
	ShipBuilder::Client::resetSlaveName();
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
		if(!root["MapBounds"].isNull())
		{
			const Json::Value boundsList = root["MapBounds"];
			m_bounds.x = (float)(boundsList[0].asInt());
			m_bounds.y = (float)(boundsList[1].asInt());
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
			float angle = Math::toDeg(atan2(spawn.y, spawn.x) + (pi / 2.f));

			ChunkDataMessage chunkMessageData;

			chunkMessageData.blueprintName = it->ship;
			chunkMessageData.coordinates = spawn;
			chunkMessageData.rotation = angle;
			chunkMessageData.team = (int)it->team;
			chunkMessageData.needsController = true;
			chunkMessageData.aiControlled = false;

			ShipBuilder::Client::createChunk(chunkMessageData);
		}

		/**Load Local Player Overlay**/
		game.getLocalPlayer().loadOverlay("overlayconfig");

		/**Set Local Controller**/
		game.getLocalPlayer().setController(data.localController);

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

				ShipBuilder::Client::createChunk(chunkMessageData);
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
}
void Universe::createControllers(Team team, bool isAnAI, const String& slaveName, int& controller, int& aiPos)
{
	//Print << "\n" << slaveName;
	//dout << FILELINE;
	controller = m_spControlFactory->addController(slaveName);

	if(isAnAI && !game.getNwBoss().isClient())
	{
		//assert(Print << "\n" << slaveName << " controlled by " << m_spControlFactory->getSize() - 1);
		sptr<ShipAI> ai = sptr<ShipAI>(new ShipAI(team, controller));
		aiPos = m_shipAI.insert(ai);
	}
}
Universe::Universe(const IOComponentData& rData) : m_io(rData, &Universe::input, this), m_physWorld(Vec2(0, 0))
{
	const Money defaultTickMoney = 1;
	const float moneyTickTime = 1.f;
	const int minTeam = (int)Team::One;
	const int maxTeam = (int)Team::Four;

	m_velocityIterations = 1;
	m_positionIterations = 1;
	m_timeStep = 1.0f / 120.0f;///LOAD FROM FILE

	m_spBPLoader = sptr<BlueprintLoader>(new BlueprintLoader);//MUST BE AFTER IO
	//m_spSlaveLocator = sptr<SlaveLocator>(new SlaveLocator);
	m_spBatchLayers = sptr<BatchLayers>(new BatchLayers);
	m_spGfxUpdater = sptr<GraphicsComponentUpdater>(new GraphicsComponentUpdater);
	m_spControlFactory = sptr<ControlFactory>(new ControlFactory);
	m_spProjMan = sptr<ProjectileMan>(new ProjectileMan);

	/**IO**/
	m_spUniverseIO = sptr<IOManager>(new IOManager(true, true));
	m_spUniverseIO->give(&m_io);
	m_spUniverseIO->give(&game.getLocalPlayer().getIOComp());
	/**IO**/

	//how often are people given capture rewards?
	m_spDecorEngine.reset(new DecorationEngine);
	m_spMoneyTimer.reset(new Timer(this->getTime()));
	m_spMoneyTimer->setCountDown(moneyTickTime);
	m_restartedMoneyTimer = false;

	for(int i = minTeam; i <= maxTeam; ++i)
		m_income[(Team)i] = defaultTickMoney;


	/**PHYControlCS**/
	m_paused = false;
	m_skippedTime = game.getTime();
	m_pauseTime = m_skippedTime;

	m_inc = 10;
	m_currentBed = Vec2(-10000, 10000);
	m_bounds = Vec2(10000, 10000);

	m_physWorld.SetContactListener(&m_contactListener);
	m_physWorld.SetDebugDraw(&m_debugDraw);
	m_debugDraw.SetFlags(b2Draw::e_shapeBit);
	/**PHYControlCS**/

	m_debugDrawEnabled = false;

	m_realTime = game.getTime();
}
Universe::~Universe()
{
	m_capturePoints.clear();
	//Print << "\nUniverse Destroying...";
	game.getLocalPlayer().universeDestroyed();
	//Print << "\nEnd.";
}
int Universe::getGameObjectPosition(String& name)
{
	for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		if(it->get() != nullptr)
			if((**it).m_io.getName() == name)
				return (it - m_goList.begin());
	}
	return -1;
}
sptr<GameObject> Universe::getGameObject(int pos)
{
	return m_goList[pos];
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
Factory<GameObject>& Universe::getGameObjects()
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
		if(it->get() != nullptr)
			(*it)->updateDecision();
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
/// Where we call prePhysUpdate on all GameObjects
/// </summary>
void Universe::prePhysUpdate()
{
	if(!m_paused)
	{
		for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
			if(it->get() != nullptr)
				(*it)->prePhysUpdate();

		m_spProjMan->prePhysUpdate();
	}
}
void Universe::changeTeamMoney(Team team, Money money)
{
	this->m_income[team] += money;
}
void Universe::physUpdate()
{
	if(!m_paused)
	{
		m_physWorld.Step(m_timeStep, m_velocityIterations, m_positionIterations);
	}
}
/// <summary>
/// Where we call postPhysUpdate on all GameObjects
/// </summary>
void Universe::postPhysUpdate()
{
	if(!m_paused)
	{
		for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
			if(it->get() != nullptr)
				(*it)->postPhysUpdate();

		m_spProjMan->postPhysUpdate();

		for(auto it = hazardFields.begin(); it != hazardFields.end(); ++it)
			if(it->get() != nullptr)
				(**it).update();
	}
}

wptr<Chunk> Universe::getNearestChunk(const Vec2& target, const Chunk* exception, std::list<Team> validTeams)
{
	float prevDist = -1;
	wptr<Chunk> closest;
	for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		sptr<Chunk> object = std::dynamic_pointer_cast<Chunk>(*it);
		if(object != nullptr && object.get() != exception && listContains(validTeams, object->getBodyComponent().getTeam()))
		{
			Vec2 dif = target - object->getBodyPtr()->GetPosition();
			float dist = dif.len();
			if(dist < prevDist || prevDist == -1)
			{
				prevDist = dist;
				closest = object;
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
	if(!m_restartedMoneyTimer)
	{
		m_spMoneyTimer->restartCountDown();
		m_restartedMoneyTimer = true;
	}

	if(game.getNwBoss().getNWState() == NWState::Server)
		if(m_spMoneyTimer->isTimeUp())
		{
			for(auto it = m_income.cbegin(); it != m_income.cend(); ++it)
				m_moneyTotals[it->first] += it->second;

			List<sptr<Connection> > cons = game.getNwBoss().getConnections();
			for(auto it = cons.begin(); it != cons.end(); ++it)
				(**it).changeMoney(m_income[(**it).getTeam()]);

			//also give money to host!
			game.getLocalPlayer().changeMoney(m_income[game.getLocalPlayer().getTeam()]);
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
int Universe::add(GameObject* pGO)
{
	return m_goList.insert(sptr<GameObject>(pGO));
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
		int controller = game.getLocalPlayer().getController();
		if(controller != -1)
		{
			auto con = m_spControlFactory->getController(controller);
				
			auto pos = con->getChunk()->getBodyComponent().getPosition();
			float maxZoom = game.getLocalPlayer().getCamera().m_maxZoom * 0.4f;
			float size = (float)game.getWindow().getSize().x / scale;
			m_spDecorEngine->initSpawns(pos, Vec2(maxZoom* size, maxZoom* size));
		}

		game.getUniverse().getControllerFactory().setAllNonLocallyControlled();
	}
	else if(rCommand == "createChunkCommand")
	{
		String slaveName;
		ChunkDataMessage data;
		data.unpack(rData);

		if(data.needsController)
			slaveName = ShipBuilder::Client::getNextSlaveName();


		auto chunkData(m_spBPLoader->getChunkSPtr(data.blueprintName)->clone());
		chunkData->bodyComp.coords = data.coordinates;
		chunkData->bodyComp.rotation = data.rotation;
		chunkData->team = (Team)data.team;
		chunkData->ioComp.name = slaveName;// data.slaveName;


		//important this happens before creation of the controller
		int chunkIndex = add(chunkData->generate(this));

		int controller = -1;
		int ai = -1;
		if(data.needsController)
			createControllers((Team)data.team, data.aiControlled, slaveName, controller, ai);// data.slaveName;

		Chunk* chunk = dynamic_cast<Chunk*>(m_goList.get(chunkIndex).get());
		chunk->universePosition = chunkIndex;
		chunk->m_controller = controller;
		chunk->m_shipAI = ai;
	}
	else if(rCommand == "killChunkCommand")
	{
		int position;
		rData >> position;

		if(position < m_goList.size())
		{
			auto chunk = dynamic_cast<Chunk*>(m_goList[position].get());
 			m_goList.free(position);
			game.getLocalPlayer().getCamera().shake(0.5, 60, 0.5);
		}
	}
	else
	{
		///ERROR LOG
		Print << m_io.getName() << ":[" << rCommand << "] not found." << FILELINE;
	}
}
bool Universe::isClear(Vec2 position, float radius, const Chunk* exception)
{
	auto nearest = getNearestChunk(position, exception);
	if(auto nearestChunk = nearest.lock())
	{
		float nearestRad = nearestChunk->getRadius();
		float dist = (nearestChunk->getBodyPtr()->GetPosition() - position).Length();
		if(dist < (nearestRad + radius))
			return false;
		else
			return true;
	}
	return true;
}
Vec2 Universe::getAvailableSpawn(Team team, float radius, const Chunk* exception)
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
