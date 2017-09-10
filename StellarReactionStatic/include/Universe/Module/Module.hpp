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
#include "Debugging.hpp"
#include "PoolChanger.hpp"

struct ModuleData;


/// Basic building block of Chunks and physical objects.
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
	virtual void setAim(const Vec2& rTarget);
	/// <summary>
	/// Get center of this module with respect to center of body.
	/// </summary>
	const Vec2& getOffset() const;
	///Get title of module.
	const String& getTitle() const;
	///Get name of module.
	const String& getName() const;
	///Set stealth to on or off.
	virtual void toggleStealth(bool toggle);
	///Heal to full health.
	virtual void healToMax();
	///Get a reference to the fixture component of this module.
	const FixtureComponent& getFixtureComponent();

	IOComponent m_io;
protected:
	virtual void input(String rCommand, sf::Packet rData);
	virtual void pack(sf::Packet& rPacket);
	virtual void unpack(sf::Packet& rPacket);
	virtual void startContactCB(FixtureComponent* pOther);
	virtual void endContactCB(FixtureComponent* pOther);

	FixtureComponent m_fix;

	PoolChanger m_poolChanger;
	Pool<Missiles>* m_pMissilePool;
	Pool<Ballistic>* m_pBallisticPool;
	Pool<Energy>* m_pEnergyPool;
	Pool<float>* m_pZoomPool;

	Timer m_stealthTimer; //timer used to turn stealth mode off;

	String m_title;//how the game refers to it
	String m_name;//what gets displayed to player
	Chunk* m_parentChunk;
	int m_collisionDamage;
private:
};

/// Blueprint for Module
struct ModuleData : public BlueprintData
{
	ModuleData() :
		name("defaultName"),
		collisionDamage(0),
		ioComp(&game.getUniverse().getUniverseIO()),
		nwComp(),
		fixComp(),
		chunkParent(NULL)
	{
		title = "MODULE_DEFAULT_TITLE";
	}

	Chunk* chunkParent;
	String name;//what gets displayed to player
	Resources cost;//how much does this cost?
	int collisionDamage;//how much damage we apply to someone who collides with us

	IOComponentData ioComp;
	NetworkComponentData nwComp;
	FixtureComponentData fixComp;
	PoolCollection pools;


	///Create Module object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		WARNING;
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

protected:
	virtual void inherit(const ModuleData& parent)
	{
		*this = parent;
	}
};
