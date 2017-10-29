#include "Range.hpp"
#include "JSON.hpp"


void RangeData::loadJson(const Json::Value& root)
{
	GETJSON(Max);
	GETJSON(Min);
	GETJSON(Value);
}

void RangeModifier::loadJson(const Json::Value& root)
{
	GETJSON(ModifierPerSecond);
	RangeData::loadJson(root);
}




