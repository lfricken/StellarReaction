#ifndef STEALTH_HPP
#define STEALTH_HPP

#include "ShipModule.hpp"

struct StealthData;

class Stealth : public ShipModule
{
public:
	Stealth(const StealthData& rData);
	virtual ~Stealth();

	virtual void postPhysUpdate();

	void directive(std::map<Directive, bool>& rIssues);

protected:
private:
	float m_eConsump;
	bool out_of_energy;
};


struct StealthData : public ShipModuleData
{
	StealthData() :
		ShipModuleData(),
		energyConsumption(5)
	{
		baseDecor.texName = "radar/radar_base.png";
		baseDecor.animSheetName = "radar/radar_base.acfg";
	}

	float energyConsumption;

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		StealthData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Stealth(copy);
	}
	virtual ModuleData* clone() const
	{
		return new StealthData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, StealthData);
};



#endif // STEALTH_HPP
