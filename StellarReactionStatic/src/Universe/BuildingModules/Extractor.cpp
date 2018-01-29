#include "Extractor.hpp"


void ExtractorData::loadJson(const Json::Value& root)
{
	ShipModuleData::loadJson(root);
}
Extractor::Extractor(const ExtractorData& rData) : ShipModule(rData)
{

}
Extractor::~Extractor()
{

}
