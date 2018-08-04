#ifndef PLATING_HPP
#define PLATING_HPP

#include "ShipModule.hpp"

struct PlatingData;

/// Designed to take damage and have high Armor.
class Plating : public ShipModule
{
public:
	Plating(const PlatingData& rData);
	virtual ~Plating();

protected:
private:
};

/// Blueprint for Plating.
struct PlatingData : public ShipModuleData
{
	PlatingData() :
		ShipModuleData()
	{
	}
	///Create CaptureGraphics object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Plating, PlatingData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new PlatingData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};



#endif // PLATING_HPP