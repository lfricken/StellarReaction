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

using namespace std;

Universe::Universe(const IOComponentData& rData) : m_io(rData, &Universe::input, this), m_physWorld(b2Vec2(0,0))
{
	m_spBPLoader = sptr<BlueprintLoader>(new BlueprintLoader);//MUST BE AFTER IO
	m_spSlaveLocator = sptr<SlaveLocator>(new SlaveLocator);
	m_spBatchLayers = sptr<BatchLayers>(new BatchLayers);
	m_spGfxUpdater = sptr<GraphicsComponentUpdater>(new GraphicsComponentUpdater);
	m_spControlFactory = sptr<ControlFactory>(new ControlFactory);
	m_spProjMan = sptr<ProjectileMan>(new ProjectileMan);

	/**IO**/
	m_spUniverseIO = sptr<IOManager>(new IOManager(true));
	m_spUniverseIO->give(&m_io);
	m_spUniverseIO->give(&game.getLocalPlayer().getIOComp());
	/**IO**/




	/**PHYControlCS**/
	m_paused = false;
	m_skippedTime = game.getTime();
	m_pauseTime = m_skippedTime;

	m_velocityIterations = 1;
	m_positionIterations = 1;
	m_timeStep = 1.0f/120.0f;///LOAD FROM FILE

	m_inc = 10;
	m_currentBed = b2Vec2(-10000,10000);

	m_physWorld.SetContactListener(&m_contactListener);
	m_physWorld.SetDebugDraw(&m_debugDraw);
	m_debugDraw.SetFlags(b2Draw::e_shapeBit);
	/**PHYControlCS**/

	m_debugDrawEnabled = false;
}
Universe::~Universe()
{
	cout << "\nUniverse Destroying...";
	game.getLocalPlayer().universeDestroyed();
	cout << "\nEnd.";
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
/// <returns></returns>
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
			(*it)->prePhysUpdate();
		m_spProjMan->prePhysUpdate();
	}
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
		m_spProjMan->postPhysUpdate();
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
		m_skippedTime += game.getTime()-m_pauseTime;
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
	m_spBPLoader->storeRoster(bpDir);
}
void Universe::loadLevel(const std::string& levelDir, int localController, const std::string& bluePrints, const std::vector<std::string>& rControllerList)//loads a level using blueprints
{
	loadBlueprints(bluePrints);

	string configFile = "level.lcfg";
	string modDir = "mods/";

	ifstream level(levelDir+configFile, std::ifstream::binary);
	Json::Reader reader;
	Json::Value root;
	bool parsedSuccess = reader.parse(level, root, false);


	if(!parsedSuccess)
	{
		cout << "\nProblem Parsing [" << levelDir+configFile << "].";
		///error log
		return;
	}
	else
	{
		/**ADDITIONAL BLUEPRINTS**/
		if(!root["AdditionalBlueprints"].isNull())
		{
			const Json::Value bpList = root["AdditionalBlueprints"];
			for(auto it = bpList.begin(); it != bpList.end(); ++it)
			{
				m_spBPLoader->storeRoster(modDir+it->asString());
			}
		}
		else
		{
			cout << FILELINE;
			///ERROR LOG
		}

		/**CHUNKS**/
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
				else if(!(*it)["ClassName"].isNull())
				{
					spCnk.reset(m_spBPLoader->loadChunk(*it)->clone());
				}
				else
				{
					cout << "\n" << FILELINE;
					///ERROR LOG
				}
				add(spCnk->generate());
			}
		}
	}


	/**HARD CODED**/
	/** Evan - please leave this for now
	    ChunkData chunkdata_1;
	    chunkdata_1.bodyComp.coords = b2Vec2(-2,5);
	    chunkdata_1.ioComp.name = "hard_coded_chunk";
	    chunkdata_1.zoomData.startValue = 10;

	    ThrusterData thrust;
	    thrust.force = 40;
	    thrust.torque = 20;
	    thrust.energyConsumption = 0.1;
	    thrust.fixComp.offset = b2Vec2(0,0);
	    chunkdata_1.moduleData.push_back(sptr<ModuleData>(new ThrusterData(thrust)));

	    SensorData sens;
	    sens.fixComp.offset = b2Vec2(3,3);
	    chunkdata_1.moduleData.push_back(sptr<ModuleData>(new SensorData(sens)));

	    ReactorData data;
	    data.fixComp.offset = b2Vec2(1,0);
	    chunkdata_1.moduleData.push_back(sptr<ModuleData>(new ReactorData(data)));

	    CapacitorData data3;
	    data3.fixComp.offset = b2Vec2(-2,0);
	    chunkdata_1.moduleData.push_back(sptr<ModuleData>(new CapacitorData(data3)));

	    TurretData data4;
	    data4.fixComp.offset = b2Vec2(-2,1);
	    LaserWeaponData* pWep = new LaserWeaponData;
	    pWep->beamColor = sf::Color::Red;
	    pWep->damage = 100;
	    pWep->beamWidth = 32;
	    data4.startWep.reset(pWep);
	    //chunkdata_1.moduleData.push_back(sptr<ModuleData>(new TurretData(data4)));

	    TurretData data6;
	    data6.fixComp.offset = b2Vec2(0,3);
	    BallisticWeaponData* pBall = new BallisticWeaponData;
	    data6.startWep.reset(pBall);
	    chunkdata_1.moduleData.push_back(sptr<ModuleData>(new TurretData(data6)));

	    TurretData* pData5 = (TurretData*)m_spBPLoader->getModuleSPtr("DefaultTurret")->clone();
	    pData5->fixComp.offset = b2Vec2(0, 2);
	    //chunkdata_1.moduleData.push_back(sptr<ModuleData>(new TurretData(*pData5)));

	    add(chunkdata_1.generate());

	        **/


	//CHOSE A CONTROLLER MANUALLY
	//localController = 0;
	//std::vector<std::string> manualController;
	// manualController.push_back("hard_coded_chunk");
	/**HARD CODED**/


	game.getLocalPlayer().loadOverlay("overlayconfig");


	//getProjMan().in


	/**CONTROL**/
	m_spControlFactory->resetControllers(rControllerList);
	game.getLocalPlayer().setController(localController);
}
void Universe::add(sptr<GameObject> spGO)
{
	m_goList.push_back(spGO);
}
void Universe::add(GameObject* pGO)
{
	m_goList.push_back(sptr<GameObject>(pGO));
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
