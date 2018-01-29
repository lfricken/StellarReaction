#include "Replicator.hpp"


void ReplicatorData::loadJson(const Json::Value& root)
{
	ShipModuleData::loadJson(root);
}
ExtractorModule::ExtractorModule(const ExtractorModuleData& rData) : ShipModule(rData)
{

}
ExtractorModule::~ExtractorModule()
{

}
