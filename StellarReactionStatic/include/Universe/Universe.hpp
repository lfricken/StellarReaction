#pragma once

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "UniversalContactListener.hpp"
#include "DebugDraw.hpp"
#include "ControlFactory.hpp"
#include "Money.hpp"
#include "Scoreboard.hpp"
#include "ChunkSpawner.hpp"
#include "ShipAI.hpp"
#include "LaunchGame.hpp"
#include "NonCopyable.hpp"
#include "Team.hpp"
#include "Clock.hpp"
#include "Resources.hpp"
#include "BuildBounds.hpp"
#include "AINode.hpp"
#include "Lanes.hpp"

class BatchLayers;
class GraphicsComponentUpdater;
class IOManager;
class Chunk;
class BlueprintLoader;
class Decoration;
class ControlFactory;
class ProjectileMan;
class BodyComponent;
class Scoreboard;
class DecorationEngine;
class ModuleParent;
enum class UpgradeType;

/// Contains everything in the "world". Relaunching a game totally recreates this object.
class Universe : public Clock, NonCopyable
{
public:
	Universe(const IOComponentData& rData);
	void postConstructor();
	virtual ~Universe();

	/// Return the ControlFactory for this Universe.
	ControlFactory& getControllerFactory();
	/// Return the SlaveLocator for this Universe.
	/*SlaveLocator& getSlaveLocator();*/
	/// Return the BatchLayers for this Universe.
	BatchLayers& getBatchLayers();
	/// Return the GraphicsComponentUpdater for this Universe.
	GraphicsComponentUpdater& getGfxUpdater();
	/// Return the Universe level IOManager.
	IOManager& getUniverseIO();
	/// Return ProjectileMan for this Universe.
	ProjectileMan& getProjMan();
	/// Return the DecorationEngine for this Universe.
	DecorationEngine& getDecors();
	Factory<Chunk>& Universe::getChunks();
	Factory<ShipAI>& getShipAI();
	/// Return the Box2D world for this Universe.
	b2World& getWorld();
	/// Return the BlueprintLoader for this Universe.
	BlueprintLoader& getBlueprints();
	/// Return the Scoreboard for this Universe.
	Scoreboard& getScoreboard();
	/// Return the bounds of this Universe.
	const Vec2& getBounds() const;
	/// Set the bounds of the Universe. Leaving bounds pushes you back to the getGame()->
	void setBounds(const Vec2& bounds);

	/// Return the physics time step. (how much the b2World's time increment is)
	float getTimeStep() const;
	/// Called before the Box2D b2World step. Gets called on the list of Chunk and Projectile
	void prePhysUpdate();
	/// Called to step the physics world.
	void physUpdate();
	/// Called after the Box2D b2World step. Gets called on the list of Chunk and Projectile
	void postPhysUpdate();
	/// Send money out to all the teams.
	void teamMoneyUpdate();
	/// Updated the positions of decorations to simulate paralax.
	void updateDecorationPosition(const Vec2& rCameraPos, float zoom);
	/// Tell the AI to determine
	void updateShipAI();


	/// Is debug draw on or not?
	bool debugDraw() const;
	/// How much time has passed. Accounts pausing.
	float getTime() const;
	/// Sets the time so universe doesn't have to ask for it.
	void setTime(float time);
	/// Set whether the Universe is paused.
	void togglePause(bool pause);
	/// Toggle whether the Universe is paused.
	void togglePause();
	/// Return whether the Universe is paused.
	bool isPaused();
	/// Set whether debug draw is on or not.
	void toggleDebugDraw();
	/// Find Chunk that is on one of the specified teams
	Chunk* getNearestChunk(const Vec2& target, const ModuleParent* exception = nullptr, std::list<Team> validTeams = std::list<Team>());

	/// Get a bed positions for a Chunk to sleep at.
	Vec2 getBed();
	/// Someone is no longer sleeping at a position. Other people can sleep there.
	void addBed(const Vec2& rBed);
	/// Load a level using Blueprints.
	void loadLevel(const GameLaunchData& data);

	/// Return list of Chunk.
	//List<sptr<Chunk> > getgoList();
	/// Used to create impact particles.
	void spawnParticles(const String& particleBP, const Vec2& pos, const Vec2& dir, const Vec2& transverse);

	UpgradeType chooseUpgradeType();
	String chooseBPtoUpgrade();

	/// Return whether a position is clear of other objects.
	bool isClear(const Vec2& position, float radius, const ModuleParent* exception);
	/// Return a spawn point for this team.

	bool started = true; //tell whether this is the initial universe created by Game, or a later one
	Vec2 getAvailableSpawn(Team team, float radius, const ModuleParent* exception);

	/// Add a Chunk to the Universe, return its index
	int add(sptr<Chunk> pGO);

	int getChunkPosition(String& name);
	Chunk* getChunk(int pos);

	const Resources& getTeamResources(Team team) const;
	bool canBuildAtLocation(String chunkBPName, Vec2 pos) const;

	Vec2 getLaneTarget(Team team, Lane lane, const Vec2& pos) const;

protected:
	bool listContains(std::list<Team> teams, Team value);
	Chunk* gameObject(int index);
	void loadBlueprints(const String& bluePrints);//loads blueprints
	void input(String rCommand, sf::Packet rData);

private:
	friend class NetworkBoss;
	void pack(sf::Packet& data);
	void unpack(sf::Packet& data);

	Map<String, List<BuildBounds> > m_buildBounds;
	/**Money**/
	std::map<Team, Resources> m_teamResources;
	std::map<Team, Resources> m_teamIncome;//how many resources does each team get per time

	/// Change the Money a team gets each tick.
	void changeTeamResources(Team team, Resources money);
	void changeTeamIncome(Team team, Resources money);
	/**Atomic Actions**/

	/// <summary>
	/// Create a controller, decide whether it's controlled by an AI, and give it a ship to control.
	/// </summary>
	void createControllers(Team team, bool isAnAI, const String& slaveName, int& controller, int& aiPos);

	int m_localController;

	/**Atomic Actions**/

	sptr<NetworkComponent> m_nw;


	/**SLEEP**/
	List<Vec2> m_beds;
	int m_inc;
	Vec2 m_currentBed;
	/**SLEEP**/

	/**TIME**/
	float m_realTime; // actual time since application start
	float m_pauseTime; // when the last pause started
	float m_skippedTime; // how much time has been skipped from being paused?
	bool m_paused; // true if the universe is paused
	/**TIME**/

	/**PHYSICS**/
	float m_timeStep;
	int m_velocityIterations;
	int m_positionIterations;

	UniversalContactListener m_contactListener;
	DebugDraw m_debugDraw;
	b2World m_physWorld;
	/**PHYSICS**/

	sptr<Scoreboard> m_scoreboard;
	sptr<ControlFactory> m_spControlFactory;
	sptr<BlueprintLoader> m_spBPLoader;
	//	sptr<SlaveLocator> m_spSlaveLocator;//list of all slaves
	sptr<BatchLayers> m_spBatchLayers;
	sptr<GraphicsComponentUpdater> m_spGfxUpdater;
	sptr<IOManager> m_spUniverseIO;//manages IO for the game objects
	sptr<ProjectileMan> m_spProjMan;//manages projectiles
	sptr<DecorationEngine> m_spDecorEngine;//list of decorations for the world

	std::map<Team, List<Vec2> > m_spawnPoints;//places for people to spawn, int is team

	bool m_allModulesUnlocked;
	bool m_restartedMoneyTimer;
	sptr<Timer> m_spMoneyTimer;//how long to wait for each money gift

	/**Ships, AI**/
	Factory<Chunk> m_goList;// must be AFTER universe graphics//list of game objects that WE need to keep track of
	Factory<ShipAI> m_shipAI;

	List<List<Vec2> > m_lanes;

	/**Hazards**/
	List<sptr<ChunkSpawner> > hazardFields;

	IOComponent m_io;
	float m_lastTime;//used for update method//cant use timer because timer references us!
	bool m_debugDrawEnabled;
	Vec2 m_bounds;
};
