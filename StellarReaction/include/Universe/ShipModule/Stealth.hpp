#ifndef STEALTH_HPP
#define STEALTH_HPP

#include "ShipModule.hpp"

struct StealthData;

class Stealth : public ShipModule
{
public:
	Stealth(const StealthData& rData);
	virtual ~Stealth();

	void directive(std::map<Directive, bool>& rIssues);
};

struct StealthData : ShipModuleData 
{
	int duration;

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
};

#endif //STEALTH_HPP