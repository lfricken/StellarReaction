#ifndef REACTOR_HPP
#define REACTOR_HPP

#include "ShipModule.hpp"

struct ReactorData;

class Reactor : public ShipModule
{
public:
	Reactor(const ReactorData& rData);
	virtual ~Reactor();

	virtual void prePhysUpdate();
	virtual void postPhysUpdate();

	void directive(std::map<Directive, bool>& rIssues);
	void respawn();

protected:
private:
	Energy m_rate;//energy per second
	Timer m_respawnTimer;
	bool m_respawned;
	float m_respawnTime;
};


struct ReactorData : public ShipModuleData
{
	ReactorData() :
		ShipModuleData(),
		rate(5),// J/s
		respawnTime(3)
	{
		baseDecor.texName = "reactor/reactor_base.png";
		baseDecor.animSheetName = "reactor/reactor_base.acfg";
	}

	Energy rate;// J/s
	float respawnTime;

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		ReactorData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Reactor(copy);
	}
	virtual ModuleData* clone() const
	{
		return new ReactorData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, ReactorData);
};



#endif // REACTOR_HPP
