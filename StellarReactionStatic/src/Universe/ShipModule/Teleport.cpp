#include "Teleport.hpp"
#include "Controller.hpp"
#include "Player.hpp"
#include "JSON.hpp"



void TeleportData::loadJson(const Json::Value& root)
{
	GETJSON(energyConsumption);
	GETJSON(teleRange);
	GETJSON(teleCooldown);
	ShipModuleData::loadJson(root);
}
Teleport::Teleport(const TeleportData& rData) : ShipModule(rData)
{
	m_eConsump = rData.energyConsumption;
	m_teleRange = rData.teleRange;
	m_teleCooldown = rData.teleCooldown;
	m_teleTimer.setCountDown(m_teleCooldown);
}
Teleport::~Teleport()
{

}
void Teleport::postPhysUpdate()
{

	ShipModule::postPhysUpdate();
}
void Teleport::directive(const CommandInfo& commands)
{
	Map<Directive, bool> rIssues = commands.directives;

	if(rIssues[Directive::Teleport])
	{
		if(m_pEnergyPool->getValue() > m_eConsump && m_teleTimer.isTimeUp() && isFunctioning())
		{
			//get mouse position and use that to decide how far we are teleporting
			Vec2 mousePos = game.getLocalPlayer().getMouseInWorld();
			b2Body* bod = m_parentChunk->getBodyPtr();
			Vec2 orgPos = bod->GetPosition();
			Vec2 diff = mousePos - orgPos;
			float dist = diff.len();
			Vec2 target;
			if(dist <= m_teleRange)
			{
				target = mousePos;
			}
			else
			{
				diff = diff.unit();
				diff *= m_teleRange;
				target = diff + orgPos;
			}
			//check if the destination is clear
			if(game.getUniverse().isClear(target, m_parentChunk->getRadius(), m_parentChunk))
			{
				//restart timer, consume energy, perfrom teleport
				m_teleTimer.restartCountDown();
				m_pEnergyPool->changeValue(-m_eConsump);
				bod->SetTransform(target, bod->GetAngle());
			}
		}
	}
}

