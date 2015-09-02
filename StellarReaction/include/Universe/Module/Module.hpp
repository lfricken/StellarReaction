#ifndef MODULE_HPP
#define MODULE_HPP

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

private:
};


struct ModuleData
{
	ModuleData() :
		//type(ModuleType::Broken),
		cost(1),
		ioComp(game.getUniverse().getUniverseIO()),
		nwComp(),
		fixComp()
	{

	}

	int cost;
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


#endif // MODULE_HPP
