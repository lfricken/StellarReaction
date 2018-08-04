#pragma once

#include "ShipModule.hpp"

struct MindData;

/// <summary>
/// Module that creates technology.
/// </summary>
class Mind : public ShipModule
{
public:

	Mind(const MindData& rData);
	virtual ~Mind();

protected:
private:

};

struct MindData : ShipModuleData
{
	MindData() :
		ShipModuleData()

	{
	}

	///Create Capacitor object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Mind, MindData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new MindData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};
