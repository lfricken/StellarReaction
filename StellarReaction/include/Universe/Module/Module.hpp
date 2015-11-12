#pragma once

#include "Globals.hpp"
#include "IOComponent.hpp"
#include "NetworkComponent.hpp"
#include "FixtureComponent.hpp"
#include "Universe.hpp"
#include "Pool.hpp"

enum class Directive;
struct ModuleData;

class Module
{
public:
	Module(const ModuleData& rData);
	virtual ~Module();

	virtual void prePhysUpdate() = 0;
	virtual void postPhysUpdate();
	virtual void directive(std::map<Directive, bool>& rIssues);
	virtual void setAim(const b2Vec2& rTarget);
	/// <summary>
	/// What store is the module intersecting?
	/// </summary>
	const std::string& getStore() const;
	const b2Vec2& getOffset() const;
	const std::string& getTitle() const;
	const std::string& getName() const;

protected:
	virtual void input(std::string rCommand, sf::Packet rData);
	virtual void pack(sf::Packet& rPacket);
	virtual void unpack(sf::Packet& rPacket);
	virtual void startContactCB(FixtureComponent* pOther);
	virtual void endContactCB(FixtureComponent* pOther);

	IOComponent m_io;
	NetworkComponent m_nw;
	FixtureComponent m_fix;

	Pool<Missiles>* m_pMissilePool;
	Pool<Ballistic>* m_pBallisticPool;
	Pool<Energy>* m_pEnergyPool;
	Pool<float>* m_pZoomPool;

	std::string m_title;//how the game refers to it
	std::string m_name;//what gets displayed to player
private:
};


struct ModuleData
{
	ModuleData() :
		title("defaultTitle"),
		name("defaultName"),
		cost(1),
		ioComp(game.getUniverse().getUniverseIO()),
		nwComp(),
		fixComp()
	{

	}

	std::string title;//how the game refers to it
	std::string name;//what gets displayed to player
	Money cost;//how much does this cost?

	IOComponentData ioComp;
	NetworkComponentData nwComp;
	FixtureComponentData fixComp;
	PoolCollection pools;

	virtual Module* generate(b2Body* pBody, PoolCollection stuff) const
	{
		std::cout << FILELINE;
		return NULL;
	}
	virtual ModuleData* clone() const
	{
		std::cout << FILELINE;
		return new ModuleData(*this);
	}
};
