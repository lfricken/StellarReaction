#include "Capacitor.hpp"
#include "UpgradeType.hpp"


void CapacitorData::loadJson(const Json::Value& root)
{
	ShipModuleData::loadJson(root);
}
Capacitor::Capacitor(const CapacitorData& rData) : ShipModule(rData)
{

}
Capacitor::~Capacitor()
{

}




