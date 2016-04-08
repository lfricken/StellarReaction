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
#include "DecorQuad.hpp"
#include "Convert.hpp"

using namespace std;


void Universe::loadLevel(const GameLaunchData& data)//loads a level using blueprints
{
	loadBlueprints("blueprints/");

	const string levelFile = "level.lcfg";
	const string levelFolder = "levels";
	const string thisLevelFolder = contentDir() + levelFolder + "/" + data.level + "/";
	const string modDir = "mods/";

	ifstream level(thisLevelFolder + levelFile, ifstream::binary);
	Json::Reader reader;
	Json::Value root;

	bool parsedSuccess = reader.parse(level, root, false);

	setupBackground();

	//SHIP AI TODO
	//m_shipAI.push_back(sptr<ShipAI>(new ShipAI));
	//m_shipAI.back()->setController(rControllerList.size() - 1);




	if(!parsedSuccess)
	{
		cout << "\nParse Failed [" << thisLevelFolder + levelFile << "].\n" << FILELINE;
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
		else
			cout << "\nAdditional Blueprints Null.";


		/**Spawn Points**/
		if(!root["SpawnPoints"].isNull())
		{
			const Json::Value spawnList = root["SpawnPoints"];
			for(auto it = spawnList.begin(); it != spawnList.end(); ++it)
			{
				const Json::Value pointsJson = (*it)["Points"];
				vector<b2Vec2> points;
				for(auto itPoint = pointsJson.begin(); itPoint != pointsJson.end(); ++itPoint)
				{
					points.push_back(b2Vec2((*itPoint)[0].asFloat(), (*itPoint)[1].asFloat()));
				}
				m_spawnPoints[(*it)["Team"].asInt()] = points;
			}
		}
		else
			cout << "\nSpawn Points Null.";


		/**Map Chunks**/
		sptr<ChunkData> spCnk;
		if(!root["Chunks"].isNull())
		{
			const Json::Value chunks = root["Chunks"];
			for(auto it = chunks.begin(); it != chunks.end(); ++it)
			{
				if(!(*it)["Title"].isNull())
				{
					spCnk.reset(m_spBPLoader->getChunkSPtr((*it)["Title"].asString())->clone());
					spCnk->bodyComp.coords.x = (*it)["Coordinates"][0].asFloat();
					spCnk->bodyComp.coords.y = (*it)["Coordinates"][1].asFloat();
				}
				else
					cout << "\n" << FILELINE;
				add(spCnk->generate(this));
			}
		}
		else
			cout << "\nChunks List Null.";


		/**Hazard Fields**/
		if(!root["HazardFields"].isNull())
		{
			const Json::Value spawnList = root["HazardFields"];
			for(auto it = spawnList.begin(); it != spawnList.end(); ++it)
				hazardFields.push_back(sptr<HazardField>(new HazardField(this, *it)));
		}
		else
			cout << "\nHazard Field Null.";
	}


	/**Create player ships**/
	sptr<ChunkData> spCnk;
	m_spControlFactory.reset(new ControlFactory);
	for(auto it = data.playerList.cbegin(); it != data.playerList.cend(); ++it)
	{
		spCnk.reset(m_spBPLoader->getChunkSPtr(it->ship)->clone());
		spCnk->bodyComp.coords = m_spawnPoints[it->team][it - data.playerList.cbegin()];
		spCnk->ioComp.name = it->slaveName;
		spCnk->team = it->team;
		addPlayer(spCnk->generate(this));

		m_spControlFactory->addController(it->slaveName);//add controller after we add the ship with the name
		//otherwise the controller cant find the intended ship
	}

	/**Load Local Player Overlay**/
	game.getLocalPlayer().loadOverlay("overlayconfig");


	/**Set Local Controller**/
	game.getLocalPlayer().setController(data.localController);


	/**Load Ship Into Store**/
	Message mes("ship_editor", "clear", voidPacket, 0, false);
	game.getCoreIO().recieve(mes);

	Controller* pController = &this->getControllerFactory().getController(data.localController);
	Chunk* pCnk = pController->getSlave();
	if(pCnk != NULL)
	{
		auto list = pCnk->getModules();
		for(auto it = list.cbegin(); it != list.cend(); ++it)
		{
			//cout << "\nUniverse: " << it->second.x << it->second.y;
			sf::Packet pack;
			pack << "addModule";
			pack << it->first;
			pack << (float)it->second.x;
			pack << (float)it->second.y;


			Message mes("networkboss", "sendTcpToHost", pack, 0, false);
			game.getCoreIO().recieve(mes);
		}
	}
	//else
	//	std::cout << "\nNo slave! " << FILELINE;


	/**Hazard Field Spawn Hazards**/
	for(auto it = hazardFields.begin(); it != hazardFields.end(); ++it)
		(**it).spawn();
}
Universe::Universe(const IOComponentData& rData) : m_io(rData, &Universe::input, this), m_physWorld(b2Vec2(0, 0))
{
	const Money defaultTickMoney = 1;
	const float moneyTickTime = 1.f;
	const int minTeam = 1;
	const int maxTeam = 4;

	m_velocityIterations = 1;
	m_positionIterations = 1;
	m_timeStep = 1.0f / 120.0f;///LOAD FROM FILE

	m_spBPLoader = sptr<BlueprintLoader>(new BlueprintLoader);//MUST BE AFTER IO
	m_spSlaveLocator = sptr<SlaveLocator>(new SlaveLocator);
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
	m_spMoneyTimer.reset(new Timer(this->getTime()));
	m_spMoneyTimer->setCountDown(moneyTickTime);
	m_restartedMoneyTimer = false;

	for(int i = minTeam; i <= maxTeam; ++i)
		m_captures[i] = defaultTickMoney;


	/**PHYControlCS**/
	m_paused = false;
	m_skippedTime = game.getTime();
	m_pauseTime = m_skippedTime;

	m_inc = 10;
	m_currentBed = b2Vec2(-10000, 10000);

	m_physWorld.SetContactListener(&m_contactListener);
	m_physWorld.SetDebugDraw(&m_debugDraw);
	m_debugDraw.SetFlags(b2Draw::e_shapeBit);
	/**PHYControlCS**/

	m_debugDrawEnabled = false;


}
Universe::~Universe()
{
	//cout << "\nUniverse Destroying...";
	game.getLocalPlayer().universeDestroyed();
	//cout << "\nEnd.";
}
ControlFactory& Universe::getControllerFactory()
{
	return *m_spControlFactory;
}
ProjectileMan& Universe::getProjMan()
{
	return *m_spProjMan;
}
SlaveLocator& Universe::getSlaveLocator()
{
	return *m_spSlaveLocator;
}
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
IOManager& Universe::getUniverseIO()
{
	return *m_spUniverseIO;
}

Scoreboard& Universe::getScoreboard()
{
	return *m_scoreboard;
}
void Universe::updateShipAI()
{
	for(auto it = m_shipAI.begin(); it != m_shipAI.end(); ++it)
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
		{
			(*it)->prePhysUpdate();
		}
		m_spProjMan->prePhysUpdate();

		for(auto it = m_shipDebris.begin(); it != m_shipDebris.end(); ++it)
		{
			(*it)->prePhysUpdate();
		}
	}
}
void Universe::changeTeamMoney(int team, Money money)
{
	this->m_captures[team] += money;
}
void Universe::physUpdate()
{
	if(!m_paused)
	{
		m_physWorld.Step(m_timeStep, m_velocityIterations, m_positionIterations);
		///m_projAlloc.recoverProjectiles();
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
			(*it)->postPhysUpdate();
		for(auto it = m_shipDebris.begin(); it != m_shipDebris.end(); ++it)
			(*it)->postPhysUpdate();
		m_spProjMan->postPhysUpdate();
	}
}
GameObject* Universe::getNearestChunkExcept(const b2Vec2& target, const b2Body* exception)
{
	float prevDist = -1;
	Chunk* closest = NULL;
	for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* p = it->get();
		Chunk* object = dynamic_cast<Chunk*>(p);
		if(object != NULL && object->getBodyPtr() != exception)
		{
			b2Vec2 dif = target - object->getBodyPtr()->GetPosition();
			float dist = dif.Length();
			if(dist < prevDist || prevDist == -1)
			{
				prevDist = dist;
				closest = object;
			}
		}
	}
	return closest;
}
Chunk* Universe::getNearestChunk(const b2Vec2& target, const Chunk* me)
{
	float prevDist = -1;
	Chunk* closest = NULL;
	for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* p = it->get();
		Chunk* object = dynamic_cast<Chunk*>(p);
		if(object != NULL && object != me)
		{
			b2Vec2 dif = target - object->getBodyPtr()->GetPosition();
			float dist = dif.Length();
			if(dist < prevDist || prevDist == -1)
			{
				prevDist = dist;
				closest = object;
			}
		}
	}
	return closest;
}
BodyComponent* Universe::getNearestBody(const b2Vec2& target)
{
	return &(dynamic_cast<Chunk*>(getNearestChunkExcept(target, NULL))->getBodyComponent());
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
			std::vector<sptr<Connection> > cons = game.getNwBoss().getConnections();
			for(auto it = cons.begin(); it != cons.end(); ++it)
			{
				(*it)->changeMoney(m_captures[(*it)->getTeam()]);
			}
			//also give money to host!
			game.getLocalPlayer().changeMoney(m_captures[game.getLocalPlayer().getTeam()]);
			m_spMoneyTimer->restartCountDown();
		}
}
void Universe::updateDecorationPosition(const b2Vec2& rCameraPos, float zoom)
{
	for(auto it = m_decorList.begin(); it != m_decorList.end(); ++it)
		(*it)->updateScaledPosition(rCameraPos);
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
		m_skippedTime += game.getTime() - m_pauseTime;
	else if(!m_paused && pause)//switch to paused
		m_pauseTime = game.getTime();

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
	float realTime = game.getTime();
	float universeTime;
	if(m_paused)
		universeTime = m_pauseTime - m_skippedTime;
	else
		universeTime = realTime - m_skippedTime;

	return universeTime;
}
b2Vec2 Universe::getBed()//give a position to sleep at
{
	b2Vec2 bed;
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
void Universe::addBed(const b2Vec2& rBed)//someone gave a bed back to us!
{
	m_beds.push_back(rBed);
}
void Universe::loadBlueprints(const std::string& bpDir)//loads blueprints
{
	m_spBPLoader->loadBlueprints(bpDir);
}
void Universe::setupBackground()
{

}
void Universe::add(sptr<GameObject> spGO)
{
	m_goList.push_back(spGO);
}
void Universe::add(GameObject* pGO)
{
	add(sptr<GameObject>(pGO));
}
void Universe::addDebris(GameObject* pGO)
{
	m_shipDebris.push_back(sptr<GameObject>(pGO));
}
void Universe::addPlayer(GameObject* pGO)
{
	m_playerShipList.push_back(sptr<GameObject>(pGO));
	add(pGO);
}
void Universe::clearDebris()
{
	m_shipDebris.clear();
}
void Universe::add(sptr<Decoration> pDec)
{
	m_decorList.push_back(pDec);
}
void Universe::add(Decoration* pDec)
{
	add(sptr<Decoration>(pDec));
}
void Universe::input(std::string rCommand, sf::Packet rData)
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
	else
	{
		///ERROR LOG
		cout << m_io.getName() << ":[" << rCommand << "] not found." << FILELINE;
	}
}
std::vector<sptr<GameObject>> Universe::getDebris()
{
	return m_shipDebris;
}
std::vector<sptr<GameObject> > Universe::getgoList()
{
	return m_goList;
}
std::vector<sptr<GameObject> > Universe::getPlayerShipList()
{
	return m_playerShipList;
}
bool Universe::isClear(b2Vec2 position, float radius, const b2Body* exception)
{
	Chunk* nearest = dynamic_cast<Chunk*>(getNearestChunkExcept(position, exception));
	if(nearest != NULL)
	{
		float nearestRad = nearest->getRadius();
		float dist = (nearest->getBodyPtr()->GetPosition() - position).Length();
		if(dist < (nearestRad + radius))
			return false;
		else
			return true;
	}
	return true;
}
