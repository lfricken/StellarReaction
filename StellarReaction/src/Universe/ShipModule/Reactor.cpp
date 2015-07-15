#include "Reactor.hpp"

Reactor::Reactor(const ReactorData& rData) : ShipModule(rData)
{
	m_rate = rData.rate;
}
Reactor::~Reactor()
{

}
void Reactor::prePhysUpdate()
{
	ShipModule::prePhysUpdate();
}
void Reactor::postPhysUpdate()
{
	if(functioning())
		m_pEnergyPool->changeValue(game.getUniverse().getTimeStep()*m_rate);
	ShipModule::postPhysUpdate();
}
