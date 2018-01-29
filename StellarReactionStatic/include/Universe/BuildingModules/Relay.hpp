#pragma once

#include "ShipModule.hpp"

struct RelayData;

/// <summary>
/// Module that represents the extractor.
/// </summary>
class Relay : public ShipModule
{
public:

	Relay(const RelayData& rData);
	virtual ~Relay();

protected:
private:

};

/// <summary>
/// 
/// </summary>
struct RelayData : ShipModuleData
{
	RelayData() :
		ShipModuleData()

	{
	}

	///Create Capacitor object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Relay, RelayData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new RelayData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, RelayData);
};
