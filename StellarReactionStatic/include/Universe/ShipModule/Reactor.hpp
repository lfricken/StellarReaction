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
	///Call to respawn this ship.
	void respawn();
	/// <summary>
	/// Called when a ship dies.
	/// </summary>
	void triggerDeathSequence();
protected:

private:

	/// <summary>
	/// Energy gained per second.
	/// </summary>
	Energy m_rate;

	Timer m_timer;
};

/// Bueprint for Reactor.
struct ReactorData : public ShipModuleData
{
	ReactorData() :
		ShipModuleData(),
		rate(5)// KJ/s
	{
		baseDecor.texName = "reactor/reactor_base";
	}

	// KJ/s
	Energy rate;

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
