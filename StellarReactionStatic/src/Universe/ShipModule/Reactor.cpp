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

	m_timer.setCountDown(0.25f);
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
	if(this->isFunctioning())
		m_pEnergyPool->changeValue(game.getUniverse().getTimeStep()*m_rate);

	ShipModule::postPhysUpdate();
}
void Reactor::directive(const CommandInfo& commands)
{
	Map<Directive, bool> rIssues = commands.directives;
	if(rIssues[Directive::Respawn])
	{
		if(m_timer.isTimeUp())
		{
			respawn();
			m_timer.restartCountDown();
		}
	}
}
void Reactor::respawn()
{
	sf::Packet data;
	ShipBuilder::Client::writeToPacket(m_parentChunk->m_io.getPosition(), m_parentChunk->getModules(), &data);
	ShipBuilder::Server::rebuild(data);

	triggerDeathSequence();
}
void Reactor::triggerDeathSequence()
{

}
