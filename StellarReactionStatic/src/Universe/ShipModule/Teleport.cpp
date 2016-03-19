#include "Teleport.hpp"
#include "Controller.hpp"
#include "Player.hpp"

using namespace std;

Teleport::Teleport(const TeleportData& rData) : ShipModule(rData)
{
	m_eConsump = rData.energyConsumption;
	m_teleRange = rData.teleRange;
	m_teleCD = rData.teleCD;
	m_teleTimer.setCountDown(m_teleCD);
}
Teleport::~Teleport()
{

}
void Teleport::postPhysUpdate()
{

	ShipModule::postPhysUpdate();
}
void Teleport::directive(map<Directive, bool>& rIssues)
{
	if(rIssues[Directive::Teleport])
	{
		if(m_pEnergyPool->getValue() > m_eConsump && m_teleTimer.isTimeUp() && isFunctioning())
		{
			//get mouse position and use that to decide how far we are teleporting
			b2Vec2 mousePos = game.getLocalPlayer().getMouseInWorld();
			b2Body* bod = m_parentChunk->getBodyPtr();
			b2Vec2 orgPos = bod->GetPosition();
			b2Vec2 diff = mousePos - orgPos;
			float dist = diff.Length();
			b2Vec2 target;
			if(dist <= m_teleRange)
			{
				target = mousePos;
			}
			else
			{
				diff.Normalize();
				diff *= m_teleRange;
				target = diff + orgPos;
			}
			//check if the destination is clear
			if(game.getUniverse().isClear(target, m_parentChunk->getRadius(), m_parentChunk->getBodyPtr()))
			{
				//restart timer, consume energy, perfrom teleport
				m_teleTimer.restartCountDown();
				m_pEnergyPool->changeValue(-m_eConsump);
				bod->SetTransform(target, bod->GetAngle());
			}
		}
	}
}
void TeleportData::loadJson(const Json::Value& root)
{
	if(!root["EnergyConsumption"].isNull())
		energyConsumption = root["EnergyConsumption"].asFloat();
	if(!root["TeleportRange"].isNull())
		teleRange = root["TeleportRange"].asFloat();
	if(!root["TeleportCooldown"].isNull())
		teleCD = root["TeleportCooldown"].asFloat();
	ShipModuleData::loadJson(root);
}


