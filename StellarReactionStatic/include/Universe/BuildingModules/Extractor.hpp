#pragma once

#include "ShipModule.hpp"

struct ExtractorModuleData;

/// <summary>
/// Module that represents the extractor.
/// </summary>
class ExtractorModule : public ShipModule
{
public:

	ExtractorModule(const ExtractorModuleData& rData);
	virtual ~ExtractorModule();

protected:
private:

};

/// <summary>
/// 
/// </summary>
struct ExtractorModuleData : ShipModuleData
{
	ExtractorModuleData() :
		ShipModuleData()

	{
	}

	///Create Capacitor object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<ExtractorModule, ExtractorModuleData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ExtractorModuleData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, ExtractorModuleData);
};
