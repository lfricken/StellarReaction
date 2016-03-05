#include "Reactor.hpp"
#include "Controller.hpp"

using namespace std;

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
	else{
		respawn();
	}
	ShipModule::postPhysUpdate();
}
void Reactor::directive(map<Directive, bool>& rIssues)
{
	if (rIssues[Directive::Respawn])
	{
		respawn();
	}
}

void Reactor::respawn(){
	//m_stealthTimer.setCountDown(5.0);
	//if (m_stealthTimer.isTimeUp())
	//	m_stealthTimer.restartCountDown();
	b2Vec2 sp = m_parentChunk->getHull()->getPosition();
	b2Body* ship = m_parentChunk->getBodyComponent().getBodyPtr();

	m_parentChunk->getHull()->setAlpha(0);

	ship->SetTransform(b2Vec2_zero, 0);
	ship->SetAngularVelocity(0);
	ship->SetLinearVelocity(b2Vec2_zero);

	std::vector<sptr<Module>> moduleList = m_parentChunk->getModuleList();
	for (auto it = moduleList.begin(); it != moduleList.end(); ++it)
	{
		(*it)->healToMax();
	}
	cout << "respawn";
}