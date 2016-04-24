#pragma once

#include "GameObject.hpp"
#include "BodyComponent.hpp"
#include "Pool.hpp"
#include "ClassRegister.hpp"
#include "QuadComponent.hpp"
#include "BlueprintData.hpp"
#include "NonCopyable.hpp"

struct ChunkData;
class Module;
struct ModuleData;
struct CommandInfo;
enum class Directive;
enum class Request;

/*
* Chunk Class:
* Extends GameObject to implement chunk game objects.
* Chunk game objects are:
*	A collection of box2d objects.
*	This contains the body, and fixtures are attached to us.
*	Controllers find us through SlaveLocator, and issue us commands.
*/
class Chunk : public GameObject
{
public:
	Chunk(const ChunkData& rData);
	virtual ~Chunk();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();
	////Actions to process on object after performing physics updates.
	virtual void postPhysUpdate();
	///Add a module to this chunk.
	void add(const ModuleData& rData);
	///Remove all modules from this chunk.
	void clear();
	///Get name of the chunk.
	const std::string& getName() const;
	///Set coordinates for current aim.
	void setAim(const b2Vec2& world);
	///Send a command to this target.
	void directive(const CommandInfo& commands);
	///Get a requested value.
	float get(Request value) const;
	///Get a pointer to the physics body object.
	b2Body* getBodyPtr();
	///Get a reference to the body component wrapper object.
	BodyComponent& getBodyComponent();
	///Returns the title of the module at that position, otherwise returns empty string.
	std::string hasModuleAt(const b2Vec2 offset) const;
	///Return a list of module names and positions in chunk.
	std::vector<std::pair<std::string, b2Vec2> > getModules() const;
	///Returns the graphical component for the hull.
	sptr<GraphicsComponent> getHull() const;
	///Returns the list of modules on in chunk.
	std::vector<sptr<Module>> getModuleList() const;
	///Returns the radius of this chunk.
	float getRadius();
	///Increments the number of deaths by this chunk.
	int incDeaths();
	///Get the spawn location in the world.
	b2Vec2 getSpawn();
	///Set the spawn location in the world.
	void setSpawn(float x, float y);
	///Returns safe location to spawn chunk near spawn point.
	b2Vec2 getClearSpawn();
	///Set stealth to on or off.
	void setStealth(bool stealthToggle);
	///Get stealth toggle.
	bool isStealth();
	///Get current score.
	int getScore();
	///Increment score.
	void increaseScore();

protected:
	virtual void input(std::string rCommand, sf::Packet rData);
	bool allows(const b2Vec2& rGridPos);

private:
	Pool<Ballistic> m_ballisticPool;
	Pool<Missiles> m_missilePool;
	Pool<Energy> m_energyPool;
	Pool<float> m_zoomPool;

	Timer m_timer;
	int m_slavePosition;
	BodyComponent m_body;
	std::vector<sptr<Module> > m_modules;

	std::vector<b2Vec2> m_validOffsets;
	b2Vec2 m_spawnPoint;

	sptr<GraphicsComponent> hull;
	bool m_wasThrusting;
	std::vector<sptr<GraphicsComponent> > afterburners;
	bool m_wasBoosting;
	std::vector<sptr<GraphicsComponent> > afterburners_boost;

	bool m_stealth;

	int m_thrustNoiseIndex;
	int m_boostNoiseIndex;
	int m_deaths;
	float m_radius;
};


struct ChunkData : public GameObjectData, public BlueprintData
{
	ChunkData() :
		GameObjectData(),
		bodyComp(),
		team(-784)
	{
		title = "CHUNK_DEFAULT_TITLE";

		zoomData.Min = 1;
		zoomData.Value = 1;
		zoomData.Max = 128;

		///TODO: for 
		for(float i = -5; i < 5; i += 0.5)
			for(float j = -5; j < 5; j += 0.5)
				validPos.push_back(b2Vec2(i, j));

	}

	PoolData<Missiles> missileData;
	PoolData<Ballistic> ballisticData;
	PoolData<Energy> energyData;
	PoolData<Zoom> zoomData;

	std::vector<b2Vec2> validPos;

	int team;
	BodyComponentData bodyComp;
	std::vector<sptr<const ModuleData> > moduleData;

	QuadComponentData hullSpriteData;
	std::vector<QuadComponentData> afterburnerSpriteData;
	std::vector<QuadComponentData> afterburnerThrustSpriteData;

	///Create Chunk object from this data object.
	virtual Chunk* generate(Universe* pParent) const
	{
		ChunkData copy(*this);
		copy.pParent = pParent;
		copy.ioComp.pMyManager = &pParent->getUniverseIO();
		return new Chunk(copy);
	}
	///Create new copy of this data object.
	virtual ChunkData* clone() const
	{
		return new ChunkData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
private:
	void loadModules(const Json::Value& root);
	MyType(ChunkData, ChunkData);
};
