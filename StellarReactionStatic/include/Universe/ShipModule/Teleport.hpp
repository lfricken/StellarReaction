#ifndef Teleport_HPP
#define Teleport_HPP

#include "ShipModule.hpp"

struct TeleportData;

class Teleport : public ShipModule
{
public:
	Teleport(const TeleportData& rData);
	virtual ~Teleport();

	virtual void postPhysUpdate();

	void directive(std::map<Directive, bool>& rIssues);

protected:
private:
	float m_eConsump;
	float m_teleRange;
	float m_teleCD;
	Timer m_teleTimer;
};


struct TeleportData : public ShipModuleData
{
	TeleportData() :
		ShipModuleData(),
		energyConsumption(5),
		teleRange(15),
		teleCD(20)
	{
		baseDecor.texName = "radar/radar_base.png";
		baseDecor.animSheetName = "radar/radar_base.acfg";
	}

	float energyConsumption;
	float teleRange;
	float teleCD;

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		TeleportData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Teleport(copy);
	}
	virtual ModuleData* clone() const
	{
		return new TeleportData(*this);
	}
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, TeleportData);
};



#endif // Teleport_HPP