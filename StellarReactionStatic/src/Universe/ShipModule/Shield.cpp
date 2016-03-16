#include "Shield.hpp"
#include "Controller.hpp"

using namespace std;

Shield::Shield(const ShieldData& rData) : ShipModule(rData)
{
	m_eConsump = rData.energyConsumption;
}

Shield::~Shield() {}

void Shield::directive(map<Directive, bool>& rIssues)
{
	if (rIssues[Directive::ShieldToggle])
	{
		cout << "Shield pressed." << endl;
	}
}

void ShieldData::loadJson(const Json::Value& root)
{
	if (!root["EnergyConsumption"].isNull())
		energyConsumption = root["EnergyConsumption"].asFloat();

	ShipModuleData::loadJson(root);
}