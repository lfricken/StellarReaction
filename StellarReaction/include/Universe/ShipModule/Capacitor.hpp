#ifndef CAPACITOR_HPP
#define CAPACITOR_HPP

#include "ShipModule.hpp"

struct CapacitorData;

class Capacitor : public ShipModule
{
public:
	Capacitor(const CapacitorData& rData);
	virtual ~Capacitor();

	virtual void setHealthStateHook(HealthState newState);

protected:
private:

	bool m_hasContributed;
	float m_storage;
};



struct CapacitorData : ShipModuleData
{
	CapacitorData() :
		ShipModuleData(),
		storage(100)
	{
		baseDecor.texName = "capacitor/capacitor_base.png";
		baseDecor.animSheetName = "capacitor/capacitor_base.acfg";
	}

	float storage;

	virtual Module* generate(b2Body* pBody, PoolCollection stuff) const
	{
		CapacitorData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		return new Capacitor(copy);
	}
	virtual ModuleData* clone() const
	{
		return new CapacitorData(*this);
	}
};

#endif // CAPACITOR_HPP
