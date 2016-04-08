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


	void directive(const CommandInfo& commands);
	void respawn();
protected:
	void respawn();
private:
	Energy m_rate;//energy per second
	Timer m_respawnTimer; //tracks respawn time
	float m_respawnTime; //how long after we die until we respawn
	bool m_respawned; //whether or ship has respawned
	
	Timer m_waitTimer; //tracks wait time
	float m_waitTime; //how long to freeze the ship after respawn
	bool m_waiting; //whether or not ship is waiting
};


struct ReactorData : public ShipModuleData
{
	ReactorData() :
		ShipModuleData(),
		rate(5),// J/s
		respawnTime(3),
		waitTime(1)
	{
		baseDecor.texName = "reactor/reactor_base.png";
		baseDecor.animSheetName = "reactor/reactor_base.acfg";
	}

	Energy rate;// J/s
	float respawnTime;
	float waitTime;

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
