#ifndef REACTOR_HPP
#define REACTOR_HPP

#include "ShipModule.hpp"

struct ReactorData;


/// Generates energy andn when destroyed, respawns the player at base.
class Reactor : public ShipModule
{
public:
	Reactor(const ReactorData& rData);
	virtual ~Reactor();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();
	///Actions to process on object after performing physics updates.
	virtual void postPhysUpdate();
	///Send commands to a target.
	void directive(const CommandInfo& commands);
	///Respawn this ship.
	void respawn();
protected:
private:
	///Energy per second.
	Energy m_rate;
	///Tracks respawn time
	Timer m_respawnTimer;
	///How long after we die until we respawn.
	float m_respawnTime; 
	///Whether or not the ship has respawned.
	bool m_respawned; 
	///Tracks wait time.
	Timer m_waitTimer;
	///How long to freeze the ship after respawn.
	float m_waitTime; 
	///Whether or not ship is waiting.
	bool m_waiting; 
};

/// Bueprint for Reactor.
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

	// J/s
	Energy rate;
	float respawnTime;
	float waitTime;

	///Create Reactor object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		ReactorData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Reactor(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ReactorData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, ReactorData);
};



#endif // REACTOR_HPP
