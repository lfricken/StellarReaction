#pragma once

#include "ShipModule.hpp"

struct ExtractorData;

/// <summary>
/// Module that represents the extractor.
/// </summary>
class Extractor : public ShipModule
{
public:

	Extractor(const ExtractorData& rData);
	virtual ~Extractor();

protected:
private:

};

/// <summary>
/// 
/// </summary>
struct ExtractorData : ShipModuleData
{
	ExtractorData() :
		ShipModuleData()

	{
	}

	///Create Capacitor object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Extractor, ExtractorData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ExtractorData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, ExtractorData);
};
