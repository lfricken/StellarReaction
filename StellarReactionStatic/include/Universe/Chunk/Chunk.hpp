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


/// <summary>
/// A collection of box2d objects
/// this contains the body, and fixtures are attached to us
/// controllers find us through SlaveLocator, and issue us commands
/// </summary>
class Chunk : public GameObject
{
public:
	Chunk(const ChunkData& rData);
	virtual ~Chunk();

	virtual void prePhysUpdate();
	virtual void postPhysUpdate();

	void add(const ModuleData& rData);
	void clear();

	const std::string& getName() const;

	void setAim(const b2Vec2& world);//send our aim coordinates
	void directive(const CommandInfo& commands);//send command to target
	float get(Request value) const;//return the requested value
	b2Body* getBodyPtr();
	BodyComponent& getBodyComponent();
	std::string hasModuleAt(const b2Vec2 offset) const;
	std::vector<std::pair<std::string, b2Vec2> > getModules() const;
	sptr<GraphicsComponent> getHull() const;
	std::vector<sptr<Module>> getModuleList() const;
	float getRadius();

	int incDeaths();
	b2Vec2 getSpawn();

	void setStealth(bool stealthToggle);
	bool isStealth();

	int getScore();
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
	b2Vec2 m_spawnPoint;//location of spawn


	//Evan - sprites for hull, afterburner, afterburner_thrust. need to set anims and anim speed individually
	sptr<GraphicsComponent> hull;
	bool m_wasThrusting;
	std::vector<sptr<GraphicsComponent> > afterburners;
	bool m_wasBoosting;
	std::vector<sptr<GraphicsComponent> > afterburners_boost;

	bool m_stealth;

	int m_thrustNoiseIndex;
	int m_boostNoiseIndex;
	int m_deaths;
};


struct ChunkData : public GameObjectData, public BlueprintData
{
	ChunkData() :
		GameObjectData(),
		bodyComp(),
		team(-784)//this value so an unititalized team member team value is distinct
	{
		title = "CHUNK_DEFAULT_TITLE";

		zoomData.Min = 1;
		zoomData.Value = 1;
		zoomData.Max = 128;

		//TODO: for 
		for(float i = -5; i < 5; i += 0.5)
			for(float j = -5; j < 5; j += 0.5)
				validPos.push_back(b2Vec2(i, j));

	}

	PoolData<Missiles> missileData;
	PoolData<Ballistic> ballisticData;
	PoolData<Energy> energyData;
	PoolData<Zoom> zoomData;

	std::vector<b2Vec2> validPos;//positions where a module can be added

	int team;
	BodyComponentData bodyComp;
	std::vector<sptr<const ModuleData> > moduleData;

	//Evan
	QuadComponentData hullSpriteData;
	std::vector<QuadComponentData> afterburnerSpriteData;
	std::vector<QuadComponentData> afterburnerThrustSpriteData;


	virtual Chunk* generate(Universe* pParent) const
	{
		ChunkData copy(*this);
		copy.pParent = pParent;
		copy.ioComp.pMyManager = &pParent->getUniverseIO();
		return new Chunk(copy);
	}
	virtual ChunkData* clone() const
	{
		return new ChunkData(*this);
	}
	virtual void loadJson(const Json::Value& root);
private:
	void loadModules(const Json::Value& root);
	MyType(ChunkData, ChunkData);
};
