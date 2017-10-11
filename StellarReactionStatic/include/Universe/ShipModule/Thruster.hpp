#ifndef THRUSTER_HPP
#define THRUSTER_HPP

#include "ShipModule.hpp"


struct ThrusterData;


/// Propels a ship.
class Thruster : public ShipModule
{
public:
	Thruster(const ThrusterData& rData);
	virtual ~Thruster();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate() final;
	///Actions to process on object after performing physics updates.
	virtual void postPhysUpdate() final;
	///Send commands to a target.
	void directive(const CommandInfo& commands);

protected:
private:
	/// Apply force in a direction (relative to angle of module).
	void thrust(const Vec2& rDirMultiplier);
	void torque(bool CCW);

	float m_eConsump;//per second
	float m_force;
	float m_torque;

	float m_boostThrust;
	float m_boostCostMulti;

	Vec2 m_forceVec;//the force List this tick?
	bool m_isCCW;//do we go CCW this tick?
};

/// Blueprint for Thruster.
struct ThrusterData : public ShipModuleData
{
	ThrusterData() :
		boostThrustMult(3),
		boostCostMult(40),
		energyConsumption(5),
		force(1),
		torque(1)
	{
		baseDecor.texName = "thruster/thruster";
	}

	float boostThrustMult;
	float boostCostMult;
	/// How much energy per second to consume.
	float energyConsumption;
	/// Strength.
	float force;
	/// Torque.
	float torque;

	///Create Thruster object from this data object.
	virtual Module* generate(b2Body* pBody, RangeList* ranges, Chunk* parent) const
	{
		ThrusterData copy(*this);
		copy.ranges = ranges;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Thruster(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ThrusterData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, ThrusterData);
};


#endif // THRUSTER_HPP
