#pragma once

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "UniversalContactListener.hpp"
#include "DebugDraw.hpp"
#include "ControlFactory.hpp"
#include "Money.hpp"
#include "Scoreboard.hpp"
#include "HazardField.hpp"
#include "ShipAI.hpp"
#include "LaunchGame.hpp"
#include "NonCopyable.hpp"
#include "Team.hpp"

class BatchLayers;
class GraphicsComponentUpdater;
class IOManager;
class GameObject;
class Factory;
class SlaveLocator;
class BlueprintLoader;
class Decoration;
class ControlFactory;
class ProjectileMan;
class BodyComponent;
class Scoreboard;
class DecorationEngine;

/// Contains everything in the "world". Relaunching a game totally recreates this object.
class Universe : NonCopyable
{
public:
	Universe(const IOComponentData& rData);
	virtual ~Universe();

	/// Return the ControlFactory for this Universe.
	ControlFactory& getControllerFactory();
	/// Return the SlaveLocator for this Universe.
	SlaveLocator& getSlaveLocator();
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
	/// Return the Box2D world for this Universe.
	b2World& getWorld();
	/// Return the BlueprintLoader for this Universe.
	BlueprintLoader& getBlueprints();
	/// Return the Scoreboard for this Universe.
	Scoreboard& getScoreboard();
	/// Return the bounds of this Universe.
	const Vec2& getBounds() const;
	/// Set the bounds of the Universe. Leaving bounds pushes you back to the game.
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

	/// Change the Money a team gets each tick.
	void changeTeamMoney(Team team, Money money);

	/// Is debug draw on or not?
	bool debugDraw() const;
	/// How much time has passed. Accounts pausing.
	float getTime() const;
	/// Set whether the Universe is paused.
	void togglePause(bool pause);
	/// Toggle whether the Universe is paused.
	void togglePause();
	/// Return whether the Universe is paused.
	bool isPaused();
	/// Set whether debug draw is on or not.
	void toggleDebugDraw();
	/// Find station that isnt on specified team
	Chunk* getNearestStation(const Vec2& target, Team team);
	/// Return pointer to the nearest Chunk, ignoring exception.
	Chunk* getNearestChunkExcept(const Vec2& target, const Chunk* exception);
	/// Return pointer to the nearest BodyComponent.
	BodyComponent* getNearestBody(const Vec2& target);
	/// Find Chunk that is on one of the specified teams
	Chunk* getNearestChunkOnTeam(const Vec2& target, const Chunk* exception, std::list<Team> teams);
	bool listContains(std::list<Team> teams, Team value);
	
	/// Get a bed positions for a Chunk to sleep at.
	Vec2 getBed();
	/// Someone is no longer sleeping at a position. Other people can sleep there.
	void addBed(const Vec2& rBed);
	/// Load a level using Blueprints.
	void loadLevel(const GameLaunchData& data);
	/// Add a Chunk to the Universe.
	void add(GameObject* pGO);
	/// Return list of GameObject.
	List<sptr<GameObject> > getgoList();
	/// Used to create impact particles.
	void spawnParticles(const String& particleBP, const Vec2& pos, const Vec2& dir);


	/// Return whether a position is clear of other objects.
	bool isClear(Vec2 position, float radius, const Chunk* exception);
	/// Return a spawn point for this team.

	bool started = true; //tell whether this is the initial universe created by Game, or a later one
	Vec2 getAvailableSpawn(Team team, float radius, const Chunk* exception);

protected:
	void loadBlueprints(const String& bluePrints);//loads blueprints

	void input(String rCommand, sf::Packet rData);

private:
	/**SLEEP**/
	List<Vec2> m_beds;
	int m_inc;
	Vec2 m_currentBed;
	/**SLEEP**/


	float m_pauseTime;
	float m_skippedTime;
	bool m_paused;

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
	sptr<SlaveLocator> m_spSlaveLocator;//list of all slaves
	sptr<BatchLayers> m_spBatchLayers;
	sptr<GraphicsComponentUpdater> m_spGfxUpdater;
	sptr<IOManager> m_spUniverseIO;//manages IO for the game objects
	sptr<ProjectileMan> m_spProjMan;//manages projectiles
	sptr<DecorationEngine> m_spDecorEngine;//list of decorations for the world

	std::map<Team, List<Vec2> > m_spawnPoints;//places for people to spawn, int is team

	/**Money**/
	std::map<Team, Money> m_moneyTotals;
	std::map<Team, Money> m_income;//how much money does each team get for capture points
	bool m_restartedMoneyTimer;
	sptr<Timer> m_spMoneyTimer;//how long to wait for each money gift

	/**Ships, AI**/
	List<sptr<GameObject> > m_goList;//list of game objects that WE need to keep track of
	List<sptr<ShipAI> > m_shipAI;
	List<Chunk*> m_capturePoints;

	/**Hazards**/
	List<sptr<HazardField> > hazardFields;


	IOComponent m_io;
	float m_lastTime;//used for update method//cant use timer because timer references us!
	bool m_debugDrawEnabled;
	Vec2 m_bounds;
};
