#include "Mind.hpp"


void MindData::loadJson(const Json::Value& root)
{
	ShipModuleData::loadJson(root);
}
Mind::Mind(const MindData& rData) : ShipModule(rData)
{

}
Mind::~Mind()
{

}
