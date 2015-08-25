#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "GameObject.hpp"
#include "BodyComponent.hpp"
#include "Pool.hpp"

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

	const std::string& getName() const;

	void setAim(const b2Vec2& world);//send our aim coordinates
	void directive(Directive issue);//send command to target
	float get(Request value) const;//return the requested value
	b2Body* getBodyPtr();


protected:
	virtual void input(std::string rCommand, sf::Packet rData);
private:

	Pool<Ballistic> m_ballisticPool;
	Pool<Missiles> m_missilePool;
	Pool<Energy> m_energyPool;
	Pool<float> m_zoomPool;

	int m_slavePosition;
	BodyComponent m_body;
	std::vector<sptr<Module> > m_modules;
};


struct ChunkData : public GameObjectData
{
	ChunkData() :
		GameObjectData(),
		bodyComp()
	{
		zoomData.startMin = 1;
		zoomData.startValue = 1;
		zoomData.startMax = 128;
	}

	PoolData<Missiles> missileData;
	PoolData<Ballistic> ballisticData;
	PoolData<Energy> energyData;
	PoolData<Zoom> zoomData;

	BodyComponentData bodyComp;
	std::vector<sptr<const ModuleData> > moduleData;

	virtual Chunk* generate() const
	{
		ChunkData copy(*this);
		return new Chunk(copy);
	}
	virtual ChunkData* clone() const
	{
		return new ChunkData(*this);
	}
};


#endif // CHUNK_HPP
