#include "Replicator.hpp"


void ReplicatorData::loadJson(const Json::Value& root)
{
	ShipModuleData::loadJson(root);
}
Replicator::Replicator(const ReplicatorData& rData) : ShipModule(rData)
{

}
Replicator::~Replicator()
{

}
