#pragma once

#include "GameObject.hpp"
#include "BodyComponent.hpp"
#include "Pool.hpp"
#include "ClassRegister.hpp"
#include "QuadComponent.hpp"
#include "BlueprintData.hpp"
#include "NonCopyable.hpp"
#include "Team.hpp"

struct ChunkData;
class Module;
struct ModuleData;
struct CommandInfo;
enum class Directive;
enum class Request;

/// \brief A players ship.
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
	const String& getName() const;
	///Set coordinates for current aim.
	void setAim(const Vec2& world);
	///Send a command to this target.
	void directive(const CommandInfo& commands);
	///Get a requested value.
	float get(Request value) const;
	///Get a pointer to the physics body object.
	b2Body* getBodyPtr();
	///Get a reference to the body component wrapper object.
	BodyComponent& getBodyComponent();
	///Returns the title of the module at that position, otherwise returns empty String.
	String hasModuleAt(const sf::Vector2i offset) const;
	///Return a list of module names and positions in chunk.
	List<std::pair<String, sf::Vector2i> > getModules() const;
	///Returns the graphical component for the hull.
	sptr<GraphicsComponent> getHull() const;
	///Returns the list of modules on in chunk.
	List<sptr<Module>> getModuleList() const;
	///Returns the radius of this chunk.
	float getRadius();
	///Increments the number of deaths by this chunk.
	int incDeaths();
	///Get the spawn location in the world.
	Vec2 getSpawn();
	///Set the spawn location in the world.
	void setSpawn(float x, float y);
	///Returns safe location to spawn chunk near spawn point.
	Vec2 getClearSpawn();
	///Set stealth to on or off.
	void setStealth(bool stealthToggle);
	///Get stealth toggle.
	bool isStealth();
	///Get current score.
	int getScore();
	///Increment score.
	void increaseScore();

	static void writeToPacket(int targetIOpos, const List<Pair<String, sf::Vector2i> >& modules, sf::Packet* data);
	static void readFromPacket(int* targetIOpos, List<Pair<String, sf::Vector2i> >* modules, sf::Packet data);

protected:
	virtual void input(String rCommand, sf::Packet rData);
	bool allows(const Vec2& rGridPos);

private:
	Pool<Ballistic> m_ballisticPool;
	Pool<Missiles> m_missilePool;
	Pool<Energy> m_energyPool;
	Pool<float> m_zoomPool;

	Timer m_timer;
	int m_slavePosition;
	BodyComponent m_body;
	List<sptr<Module> > m_modules;

	List<Vec2> m_validOffsets;
	Vec2 m_spawnPoint;

	sptr<GraphicsComponent> hull;
	bool m_wasThrusting;
	List<sptr<GraphicsComponent> > afterburners;
	bool m_wasBoosting;
	List<sptr<GraphicsComponent> > afterburners_boost;

	bool m_stealth;

	int m_thrustNoiseIndex;
	int m_boostNoiseIndex;
	int m_deaths;
	float m_radius;

	Vec2 m_lastAim;
};

/// Initialize a Chunk.
struct ChunkData : public GameObjectData, public BlueprintData
{
	ChunkData() :
		GameObjectData(),
		bodyComp(),
		team(Team::Alone)
	{
		title = "CHUNK_DEFAULT_TITLE";

		zoomData.Min = 1;
		zoomData.Value = 1;
		zoomData.Max = 128;

		///TODO: for 
		for(float i = -5; i < 5; i += 0.5)
			for(float j = -5; j < 5; j += 0.5)
				validPos.push_back(Vec2(i, j));

	}

	PoolData<Missiles> missileData;
	PoolData<Ballistic> ballisticData;
	PoolData<Energy> energyData;
	PoolData<Zoom> zoomData;

	List<Vec2> validPos;

	Team team;
	BodyComponentData bodyComp;
	List<sptr<const ModuleData> > moduleData;

	QuadComponentData hullSpriteData;
	List<QuadComponentData> afterburnerSpriteData;
	List<QuadComponentData> afterburnerThrustSpriteData;

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
