#include "ShipProfile.hpp"
#include "JSON.hpp"

void HardpointRequirements::loadJson(const Json::Value& root)
{
	GETJSON(weapon);
	GETJSON(engine);
	GETJSON(special);
}

void Hardpoint::loadJson(const Json::Value& root)
{
	HardpointRequirements::loadJson(root);
	GETJSON(special);
}

void ShipProfile::loadJson(const Json::Value& root)
{
	for(auto it = root.begin(); it != root.end(); ++it)
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


