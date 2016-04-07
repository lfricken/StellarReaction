#include "Reactor.hpp"
#include "Controller.hpp"
#include "BlueprintLoader.hpp"

using namespace std;

void ReactorData::loadJson(const Json::Value& root)
{
	GETJSON(rate);
	GETJSON(respawnTime);

	ShipModuleData::loadJson(root);
}
Reactor::Reactor(const ReactorData& rData) : ShipModule(rData)
{
	m_rate = rData.rate;
	m_respawnTime = rData.respawnTime;
	m_respawned = true;
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
	if(isFunctioning() && m_respawned)
		m_pEnergyPool->changeValue(game.getUniverse().getTimeStep()*m_rate);
	else
		respawn();

	ShipModule::postPhysUpdate();
}
void Reactor::directive(map<Directive, bool>& rIssues)
{
	if(rIssues[Directive::Respawn])
	{
		respawn();
	}
}
void Reactor::respawn()
{
	if (m_respawnTimer.isTimeUp() && !m_respawned)
	{
		//now do ship manipulation
		b2Body* ship = m_parentChunk->getBodyComponent().getBodyPtr();
		m_parentChunk->incDeaths();
		//move ship to spawn point, orient toward origin
		b2Vec2 spawn = m_parentChunk->getClearSpawn();// +displacement;
		float angle = atan2(spawn.y, spawn.x) + (pi / 2.f);
		ship->SetTransform(spawn, angle);
		//halt all ship movement
		ship->SetAngularVelocity(0);
		ship->SetLinearVelocity(b2Vec2_zero);
		//heal all modules to max hp
		std::vector<sptr<Module>> moduleList = m_parentChunk->getModuleList();
		for (auto it = moduleList.begin(); it != moduleList.end(); ++it)
		{
			(*it)->healToMax();
		}
		m_respawned = true;
	}
	else if (m_respawned)
	{
		m_respawnTimer.setCountDown(m_respawnTime);
		m_respawnTimer.restartCountDown();
		m_respawned = false;
	}
}
