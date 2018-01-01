#include "ShipProfile.hpp"
#include "JSON.hpp"

HardpointRequirements::HardpointRequirements()
{
	weapon = false;
	engine = false;
	special = false;
}
void HardpointRequirements::loadJson(const Json::Value& root)
{
	GETJSON(weapon);
	GETJSON(engine);
	GETJSON(special);
}

void Hardpoint::loadJson(const Json::Value& root)
{
	HardpointRequirements::loadJson(root);
	GETJSON(position);
}

void ShipProfile::loadJson(const Json::Value& root)
{
	const Json::Value& hardpointsRoot = root[NAMEOF(hardpoints)];

	if(!hardpointsRoot.isNull())
		for(auto it = hardpointsRoot.begin(); it != hardpointsRoot.end(); ++it)
		{
			hardpoints.push_back(Hardpoint());
			hardpoints.back().loadJson(*it);
		}
}

bool Hardpoint::providesRequirements(const HardpointRequirements& other) const
{
	if(other.weapon && !weapon)
		return false;
	if(other.engine && !engine)
		return false;
	if(other.special && !special)
		return false;

	return true;
}


