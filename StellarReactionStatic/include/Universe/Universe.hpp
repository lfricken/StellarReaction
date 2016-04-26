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

/// <summary>
/// A new one is instantiated when you relaunch the game
/// </summary>
class Universe : NonCopyable
{
public:
	Universe(const IOComponentData& rData);
	virtual ~Universe();

	ControlFactory& getControllerFactory();
	SlaveLocator& getSlaveLocator();
	BatchLayers& getBatchLayers();
	GraphicsComponentUpdater& getGfxUpdater();
	IOManager& getUniverseIO();
	ProjectileMan& getProjMan();
	DecorationEngine& getDecors();
	b2World& getWorld();
	BlueprintLoader& getBlueprints();
	Scoreboard& getScoreboard();
	std::vector<int> getBounds();
	void setBounds(int x, int y);

	float getTimeStep() const;
	void prePhysUpdate();
	void physUpdate();
	void postPhysUpdate();
	void teamMoneyUpdate();
	void updateDecorationPosition(const b2Vec2& rCameraPos, float zoom);
	void updateShipAI();

	void changeTeamMoney(int team, Money money);

	bool debugDraw() const;//should we draw debug or normal?
	float getTime() const;
	void togglePause(bool pause);
	void togglePause();
	bool isPaused();
	void toggleDebugDraw();
	Chunk* getNearestStation(const b2Vec2& target, int team);
	Chunk* getNearestChunkExcept(const b2Vec2& target, const Chunk* exception);
	BodyComponent* getNearestBody(const b2Vec2& target);
	Chunk* getNearestChunkOnTeam(const b2Vec2& target, const Chunk* exception, std::list<int> teams);
	bool listContains(std::list<int> teams, int value);

	b2Vec2 getBed();//give a position to sleep at
	void addBed(const b2Vec2& rBed);//someone gave a bed back to us!	
	void loadLevel(const GameLaunchData& data);//loads a level using blueprints

	void add(GameObject* pGO);

	std::vector<sptr<GameObject> > getgoList();

	bool isClear(b2Vec2 position, float radius, const Chunk* exception);
	b2Vec2 getAvailableSpawn(int team, float radius, const Chunk* exception);



protected:
	void loadBlueprints(const std::string& bluePrints);//loads blueprints

	void input(std::string rCommand, sf::Packet rData);

private:
	/**SLEEP**/
	std::vector<b2Vec2> m_beds;
	int m_inc;
	b2Vec2 m_currentBed;
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

	std::map<int, std::vector<b2Vec2> > m_spawnPoints;//places for people to spawn, int is team

	/**Money**/
	std::map<int, Money> m_captures;//how much money does each team get for capture points
	bool m_restartedMoneyTimer;
	sptr<Timer> m_spMoneyTimer;//how long to wait for each money gift

	/**Ships, AI**/
	std::vector<sptr<GameObject> > m_goList;//list of game objects that WE need to keep track of
	std::vector<sptr<ShipAI> > m_shipAI;
	std::vector<Chunk*> m_capturePoints;

	/**Hazards**/
	std::vector<sptr<HazardField> > hazardFields;


	IOComponent m_io;
	float m_lastTime;//used for update method//cant use timer because timer references us!
	bool m_debugDrawEnabled;
	std::vector<int> m_bounds;
};
