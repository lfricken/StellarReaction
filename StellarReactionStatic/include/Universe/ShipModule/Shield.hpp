#ifndef SHIELD_HPP
#define SHIELD_HPP

#include "ShipModule.hpp"

struct ShieldData;

class Shield : public ShipModule
{
public:
	Shield(const ShieldData& rData);
	virtual ~Shield();

	//virtual void postPhysUpdate();

	//void directive(std::map<Directive, bool>& rIssues);

protected:
private:
	float m_eConsump;
	bool out_of_energy;
	const float consump_per_hit = 2.0;
};

struct ShieldData : public ShipModuleData
{
	ShieldData() : ShipModuleData(), energyConsumption(5)
	{
		baseDecor.texName = "shield/shield.png";
		baseDecor.animSheetName = "shield/shield_base.acfg";
	}

	float energyConsumption;

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		ShieldData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Shield(copy);
	}
	virtual ModuleData* clone() const
	{
		return new ShieldData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, ShieldData);
};

#endif // SHIELD_HPP