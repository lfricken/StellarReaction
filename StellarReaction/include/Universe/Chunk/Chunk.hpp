#pragma once

#include "GameObject.hpp"
#include "BodyComponent.hpp"
#include "Pool.hpp"

#include "QuadComponent.hpp"

struct ChunkData;
class Module;
struct ModuleData;
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
	void directive(std::map<Directive, bool>& rIssues, bool local);//send command to target
	float get(Request value) const;//return the requested value
	b2Body* getBodyPtr();
	std::string hasModuleAt(const b2Vec2 offset) const;
	std::vector<std::pair<std::string, b2Vec2> > getModules() const;
	sptr<GraphicsComponent> getHull() const;
	std::vector<sptr<Module>> getModuleList() const;

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


	//Evan - sprites for hull, afterburner, afterburner_thrust. need to set anims and anim speed individually
	sptr<GraphicsComponent> hull;
	bool m_wasThrusting;
	std::vector<sptr<GraphicsComponent> > afterburners;
	bool m_wasBoosting;
	std::vector<sptr<GraphicsComponent> > afterburners_boost;

	int m_thrustNoiseIndex;
	int m_boostNoiseIndex;
};


struct ChunkData : public GameObjectData
{
	ChunkData() :
		GameObjectData(),
		bodyComp(),
		team(-784)
	{
		zoomData.startMin = 1;
		zoomData.startValue = 1;
		zoomData.startMax = 128;

		//TODO: for accepting
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

	//Evan
	QuadComponentData hullSpriteData;
	std::vector<QuadComponentData> afterburnerSpriteData;
	std::vector<QuadComponentData> afterburnerThrustSpriteData;


	virtual Chunk* generate(Universe* pParent) const
	{
		ChunkData copy(*this);
		copy.pParent = pParent;
		return new Chunk(copy);
	}
	virtual ChunkData* clone() const
	{
		return new ChunkData(*this);
	}
};
