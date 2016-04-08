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

class Module : NonCopyable
{
public:
	Module(const ModuleData& rData);
	virtual ~Module();

	virtual void prePhysUpdate() = 0;
	virtual void postPhysUpdate();
	virtual void directive(const CommandInfo& commands);
	virtual void setAim(const b2Vec2& rTarget);
	/// <summary>
	/// What store is the module intersecting?
	/// </summary>
	const std::string& getStore() const;
	const b2Vec2& getOffset() const;
	const std::string& getTitle() const;
	const std::string& getName() const;
	virtual void toggleStealth(bool toggle);

	virtual void healToMax();

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

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		std::cout << FILELINE;
		return NULL;
	}
	virtual ModuleData* clone() const
	{
		return new ModuleData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, ModuleData);
};
