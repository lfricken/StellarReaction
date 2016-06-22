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

	m_respawnTimer.setCountDown(rData.respawnTime);

	m_waitingToRespawn = false;
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
	m_pEnergyPool->changeValue(game.getUniverse().getTimeStep()*m_rate);

	if(!m_waitingToRespawn && m_health.isDead())
	{
		startRespawn();
	}
	if(m_waitingToRespawn && m_respawnTimer.isTimeUp())
	{
		respawn();
	}

	ShipModule::postPhysUpdate();
}
void Reactor::directive(const CommandInfo& commands)
{
	Map<Directive, bool> rIssues = commands.directives;
	if(rIssues[Directive::Respawn])
	{
		if(!m_waitingToRespawn)
			startRespawn();
	}
}
void Reactor::startRespawn()
{
	m_waitingToRespawn = true;
	m_respawnTimer.restartCountDown();
}
void Reactor::respawn()
{
	m_waitingToRespawn = false;

	//ship manipulation
	b2Body* ship = m_parentChunk->getBodyPtr();
	m_parentChunk->incDeaths();
	//move ship to spawn point, orient toward origin
	b2Vec2 spawn = m_parentChunk->getClearSpawn();// +displacement;
	float angle = atan2(spawn.y, spawn.x) + (pi / 2.f);
	ship->SetTransform(spawn, angle);
	healModules();
}
void Reactor::healModules()
{
	//heal all modules to max hp
	std::vector<sptr<Module>> moduleList = m_parentChunk->getModuleList();
	for(auto it = moduleList.begin(); it != moduleList.end(); ++it)
	{
		(*it)->healToMax();
	}
}
