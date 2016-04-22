#pragma once

#include "Globals.hpp"
#include "IOComponent.hpp"
#include "NetworkComponent.hpp"
#include "FixtureComponent.hpp"
#include "Universe.hpp"
#include "Pool.hpp"
#include "Chunk.hpp"
#include "ClassRegister.hpp"
#include "JSON.hpp"
#include "CommandInfo.hpp"
#include "NonCopyable.hpp"

struct ModuleData;
/*
* Module Class:
* Implements module game object
* Module game object is the basic building block of other game objects.
*/
class Module : NonCopyable
{
public:
	Module(const ModuleData& rData);
	virtual ~Module();
	///Pure Virtual function that processes actions on object before physics updates.
	virtual void prePhysUpdate() = 0;
	///Actions to process on object after performing physics updates.
	virtual void postPhysUpdate();
	///Send a command to a target.
	virtual void directive(const CommandInfo& commands);
	///Set coordinates for current aim.
	virtual void setAim(const b2Vec2& rTarget);
	/// What store is the module intersecting?
	const std::string& getStore() const;
	///Get offset from center of fixture component.
	const b2Vec2& getOffset() const;
	///Get title of module.
	const std::string& getTitle() const;
	///Get name of module.
	const std::string& getName() const;
	///Set stealth to on or off.
	virtual void toggleStealth(bool toggle);
	///Heal to full health.
	virtual void healToMax();
	///Get a reference to the fixture component of this module.
	const FixtureComponent& getFixtureComponent();

protected:
	virtual void input(std::string rCommand, sf::Packet rData);
	virtual void pack(sf::Packet& rPacket);
	virtual void unpack(sf::Packet& rPacket);
	virtual void startContactCB(FixtureComponent* pOther);
	virtual void endContactCB(FixtureComponent* pOther);

	IOComponent m_io;
	FixtureComponent m_fix;

	Pool<Missiles>* m_pMissilePool;
	Pool<Ballistic>* m_pBallisticPool;
	Pool<Energy>* m_pEnergyPool;
	Pool<float>* m_pZoomPool;

	Timer m_stealthTimer; //timer used to turn stealth mode off;

	std::string m_title;//how the game refers to it
	std::string m_name;//what gets displayed to player
	Chunk* m_parentChunk;
	int m_collisionDamage;
private:
};


struct ModuleData : public BlueprintData
{
	ModuleData() :
		name("defaultName"),
		collisionDamage(0),
		cost(1),
		ioComp(&game.getUniverse().getUniverseIO()),
		nwComp(),
		fixComp(),
		chunkParent(NULL)
	{
		title = "MODULE_DEFAULT_TITLE";
	}

	Chunk* chunkParent;
	std::string name;//what gets displayed to player
	Money cost;//how much does this cost?
	int collisionDamage;//how much damage we apply to someone who collides with us

	IOComponentData ioComp;
	NetworkComponentData nwComp;
	FixtureComponentData fixComp;
	PoolCollection pools;

	///Create Module object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		std::cout << FILELINE;
		return NULL;
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ModuleData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, ModuleData);
};
