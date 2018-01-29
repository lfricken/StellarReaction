#include "Relay.hpp"


void RelayData::loadJson(const Json::Value& root)
{
	ShipModuleData::loadJson(root);
}
Relay::Relay(const RelayData& rData) : ShipModule(rData)
{

}
Relay::~Relay()
{

}
