#pragma once

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "UniversalContactListener.hpp"
#include "DebugDraw.hpp"
#include "ControlFactory.hpp"
#include "Money.hpp"

#include "Hazard\HazardField.hpp"

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

/// <summary>
/// A new one is instantiated when you relaunch the game
/// </summary>
class Universe
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
	b2World& getWorld();
	BlueprintLoader& getBlueprints();

	float getTimeStep() const;
	void prePhysUpdate();
	void physUpdate();
	void postPhysUpdate();
	void teamMoneyUpdate();
	void updateDecorationPosition(const b2Vec2& rCameraPos, float zoom);

	void changeTeamMoney(int team, Money money);

	bool debugDraw() const;//should we draw debug or normal?
	float getTime() const;
	void togglePause(bool pause);
	void togglePause();
	bool isPaused();
	void toggleDebugDraw();
	BodyComponent* getNearestBody(const b2Vec2& target);

	b2Vec2 getBed();//give a position to sleep at
	void addBed(const b2Vec2& rBed);//someone gave a bed back to us!	
	void loadLevel(const std::string& level, int localController, const std::string& bluePrints, const std::vector<std::string>& rControllerList, const std::vector<std::string>& rShipTitleList, const std::vector<int>& teams);//loads a level using blueprints

	void add(sptr<GameObject> spGO);
	void add(GameObject* pGO);
	void add(sptr<Decoration> pDec);
	void add(Decoration* pDec);


	float m_pauseTime;
	float m_skippedTime;
	bool m_paused;


protected:
	void loadBlueprints(const std::string& bluePrints);//loads blueprints

	void input(std::string rCommand, sf::Packet rData);

private:
	std::vector <HazardField> hazardFields;

	//TODO
	void spawnChunk(int x, int y);


	void setupBackground();

	/**SLEEP**/
	std::vector<b2Vec2> m_beds;
	int m_inc;
	b2Vec2 m_currentBed;
	/**SLEEP**/

	/**PHYSICS**/
	float m_timeStep;
	int m_velocityIterations;
	int m_positionIterations;

	UniversalContactListener m_contactListener;
	DebugDraw m_debugDraw;
	b2World m_physWorld;
	/**PHYSICS**/

	sptr<ControlFactory> m_spControlFactory;
	sptr<BlueprintLoader> m_spBPLoader;
	sptr<SlaveLocator> m_spSlaveLocator;//list of all slaves
	sptr<BatchLayers> m_spBatchLayers;
	sptr<GraphicsComponentUpdater> m_spGfxUpdater;
	sptr<IOManager> m_spUniverseIO;//manages IO for the game objects
	sptr<ProjectileMan> m_spProjMan;//manages IO for the game objects

	std::map<int, std::vector<b2Vec2> > m_spawnPoints;//places for people to spawn, int is team
	std::map<int, Money> m_captures;//how much money does each team get for capture points
	sptr<Timer> m_spMoneyTimer;//how long to wait for each money gift

	std::vector<sptr<GameObject> > m_goList;//list of game objects that WE need to keep track of

	std::vector<sptr<Decoration> > m_decorList;//list of decorations for the world

	IOComponent m_io;
	float m_lastTime;//used for update method//cant use timer because timer references us!
	bool m_debugDrawEnabled;
};