#pragma once

#include "ShipModule.hpp"

struct ExtractorData;

/// <summary>
/// Module that changes income.
/// </summary>
class Extractor : public ShipModule
{
public:

	Extractor(const ExtractorData& rData);
	virtual ~Extractor();

protected:

	bool m_opposingTeams; // true if this change in resources is applied to the opposing teams
	Resources m_incomeChange; // how much income does this object add or subtract from its team?
private:

};

struct ExtractorData : ShipModuleData
{
	ExtractorData() :
		ShipModuleData(),
		opposingTeams(false)
	{
	}

	bool opposingTeams; // true if this change in resources is applied to the opposing teams
	Resources incomeChange; // how much income does this object add or subtract from its team just by existing?

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
};
