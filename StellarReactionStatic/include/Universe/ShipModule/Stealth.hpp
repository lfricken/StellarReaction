#ifndef STEALTH_HPP
#define STEALTH_HPP

#include "ShipModule.hpp"

struct StealthData;

/// Makes a ship invisible at the cost of energy.
class Stealth : public ShipModule
{
public:
	Stealth(const StealthData& rData);
	virtual ~Stealth();
	///Actions to process on object after performing physics updates.
	virtual void postPhysUpdate();
	///Send comands to a target.
	void directive(const CommandInfo& commands);

protected:
private:
	float m_eConsump;
	bool out_of_energy;
	const float stealth_duration = 5.0;
};

/// Blueprint for a Stealth.
struct StealthData : public ShipModuleData
{
	StealthData() :
		ShipModuleData(),
		energyConsumption(5)
	{
	}

	float energyConsumption;
	///Create Stealth object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Stealth, StealthData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new StealthData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};



#endif // STEALTH_HPP
