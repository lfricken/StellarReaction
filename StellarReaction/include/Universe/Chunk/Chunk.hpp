#ifndef CHUNK_HPP
#define CHUNK_HPP

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

	const std::string& getName() const;

	void setAim(const b2Vec2& world);//send our aim coordinates
	void directive(std::map<Directive, bool>& rIssues);//send command to target
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

	//Evan - container of decor sprite elements (ex: ship hull)
	std::vector<sptr<GraphicsComponent> > m_decors;
	//Evan - keyDown var is for afterb anim and sound
	bool keyShiftIsdown;
	bool keyUpIsdown;
	sf::SoundBuffer buffer;
	sf::Sound afterb_sound;
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

		//TODO - handle this case
		//hullData.layer = GraphicsLayer::ShipHull; //all hulls belong to layer by default
	}

	PoolData<Missiles> missileData;
	PoolData<Ballistic> ballisticData;
	PoolData<Energy> energyData;
	PoolData<Zoom> zoomData;

	BodyComponentData bodyComp;
	std::vector<sptr<const ModuleData> > moduleData;

	std::vector<QuadComponentData> hullData;

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
