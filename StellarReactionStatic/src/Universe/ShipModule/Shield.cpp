#include "Shield.hpp"
#include "Controller.hpp"

Shield::Shield(const ShieldData& rData) : ShipModule(rData)
{
	m_eConsump = rData.energyConsumption;
}

Shield::~Shield() {}

void ShieldData::loadJson(const Json::Value& root)
{
	if (!root["EnergyConsumption"].isNull())
		energyConsumption = root["EnergyConsumption"].asFloat();

	ShipModuleData::loadJson(root);
}