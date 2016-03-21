#include "Plating.hpp"

void PlatingData::loadJson(const Json::Value& root)
{
	ShipModuleData::loadJson(root);
}
Plating::Plating(const PlatingData& rData) : ShipModule(rData)
{

}
Plating::~Plating()
{

}


