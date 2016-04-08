#include "Reactor.hpp"
#include "Controller.hpp"
#include "BlueprintLoader.hpp"

using namespace std;

void ReactorData::loadJson(const Json::Value& root)
{
	GETJSON(rate);
	GETJSON(respawnTime);
	GETJSON(waitTime);

	ShipModuleData::loadJson(root);
}
Reactor::Reactor(const ReactorData& rData) : ShipModule(rData)
{
	m_rate = rData.rate;
	m_respawnTime = rData.respawnTime;
	m_waitTime = rData.waitTime;
	m_respawned = true;
	m_waiting = false;
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
	//check if we should respawn or operate normally
	if (isFunctioning() && m_respawned)
		m_pEnergyPool->changeValue(game.getUniverse().getTimeStep()*m_rate);
	else
		respawn();

	if (m_waitTimer.isTimeUp() && !m_respawned)
	{
		//stop waiting
		m_waiting = false;
		m_respawned = true;
		//heal all modules to max hp
		std::vector<sptr<Module>> moduleList = m_parentChunk->getModuleList();
		for (auto it = moduleList.begin(); it != moduleList.end(); ++it)
		{
			(*it)->healToMax();
		}
	}
	//halt all ship movement
	if (m_waiting)
	{
		m_parentChunk->getBodyPtr()->SetLinearVelocity(b2Vec2_zero);
		m_parentChunk->getBodyPtr()->SetAngularVelocity(0);
	}
	
	ShipModule::postPhysUpdate();
}
void Reactor::directive(map<Directive, bool>& rIssues)
{
	if(rIssues[Directive::Respawn])
	{
		//respawn();
		b2Vec2 pos = m_parentChunk->getBodyPtr()->GetPosition();
		cout << pos.x << " " << pos.y << "\n";
	}
}
void Reactor::respawn()
{	
	if (m_respawned && !m_waiting)
	{
		m_respawnTimer.setCountDown(m_respawnTime);
		m_respawnTimer.restartCountDown();
		m_waitTimer.setCountDown(m_waitTime + m_respawnTime);
		m_waitTimer.restartCountDown();
		m_respawned = false;
		return;
	}
	if (m_respawnTimer.isTimeUp() && !m_respawned && !m_waiting)
	{
		//ship manipulation
		b2Body* ship = m_parentChunk->getBodyPtr();
		m_parentChunk->incDeaths();
		//move ship to spawn point, orient toward origin
		b2Vec2 spawn = m_parentChunk->getClearSpawn();// +displacement;
		float angle = atan2(spawn.y, spawn.x) + (pi / 2.f);
		ship->SetTransform(spawn, angle);
		//ship is waiting now
		m_waiting = true;
		return;
	}

}
