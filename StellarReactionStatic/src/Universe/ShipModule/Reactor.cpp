#include "Reactor.hpp"
#include "Controller.hpp"
#include "BlueprintLoader.hpp"

using namespace std;

void ReactorData::loadJson(const Json::Value& root)
{
	GETJSON(rate);

	ShipModuleData::loadJson(root);
}
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
	if(isFunctioning())
		m_pEnergyPool->changeValue(game.getUniverse().getTimeStep()*m_rate);
	else
		respawn();
	if(m_debrisTimer.isTimeUp() && !m_debrisList.empty())
	{
		game.getUniverse().clearDebris();
		m_debrisList.clear();
	}
	ShipModule::postPhysUpdate();
}
void Reactor::directive(const CommandInfo& commands)
{
	Map<Directive, bool> rIssues = commands.directives;
	if(rIssues[Directive::Respawn])
	{
		respawn();
	}
}
void Reactor::respawn()
{
	//now do ship manipulation
	b2Body* ship = m_parentChunk->getBodyComponent().getBodyPtr();
	m_parentChunk->incDeaths();
	//start debris timer
	m_debrisTimer.setCountDown(3.0);
	if(m_debrisTimer.isTimeUp())
	{
		m_debrisTimer.restartCountDown();
	}
	//create debris
	//for (int i = 0; i<3; i++)
	//{
	//	//TODOD, fix strange numbers, use Random
	//	Universe* uni = &game.getUniverse();
	//	ChunkData* p = uni->getBlueprints().getChunkSPtr("Asteroid")->clone();
	//	b2Vec2 velocity = b2Vec2((rand() % 30 - 15), (rand() % 30 - 15));
	//	b2Vec2 displacement = b2Vec2(rand() % 6 - 3, rand() % 6 - 3);
	//	p->bodyComp.coords = ship->GetPosition()+ displacement;
	//	Chunk* debris = p->generate(uni);
	//	b2Body* bod = debris->getBodyPtr();
	//	bod->SetAngularVelocity((rand() % 10 - 5)*5);
	//	bod->SetAngularDamping(0.f);
	//	bod->SetLinearVelocity(velocity + ship->GetLinearVelocity());
	//	bod->SetLinearDamping(0.f);
	//	uni->addDebris(debris);
	//	m_debrisList.push_back(debris);
	//}
	//move ship to spawn point, orient toward origin
	b2Vec2 displacement = b2Vec2(static_cast<float>(rand() % 10 - 5), static_cast<float>(rand() % 10 - 5));
	b2Vec2 spawn = m_parentChunk->getSpawn() + displacement;
	float angle = atan2(spawn.y, spawn.x) + (pi / 2.f);
	ship->SetTransform(spawn, angle);
	//halt all ship movement
	ship->SetAngularVelocity(0);
	ship->SetLinearVelocity(b2Vec2_zero);
	//heal all modules to max hp
	std::vector<sptr<Module>> moduleList = m_parentChunk->getModuleList();
	for(auto it = moduleList.begin(); it != moduleList.end(); ++it)
	{
		(*it)->healToMax();
	}
}
