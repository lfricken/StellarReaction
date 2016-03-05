#include "Plating.hpp"


Plating::Plating(const PlatingData& rData) : ShipModule(rData)
{

}
Plating::~Plating()
{

}
void PlatingData::loadJson(const Json::Value& root)
{


	ShipModuleData::loadJson(root);
}


