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
};

/// Bueprint for Reactor.
struct ReactorData : public ShipModuleData
{
	ReactorData() :
		ShipModuleData()
	{
		baseDecor.texName = "reactor/reactor_base";
	}

	///Create Reactor object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Reactor, ReactorData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ReactorData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};



#endif // REACTOR_HPP
