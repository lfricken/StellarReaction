#include "Reactor.hpp"
#include "Controller.hpp"
#include "BlueprintLoader.hpp"



void ReactorData::loadJson(const Json::Value& root)
{
	ShipModuleData::loadJson(root);
}
Reactor::Reactor(const ReactorData& rData) : ShipModule(rData)
{
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

}

