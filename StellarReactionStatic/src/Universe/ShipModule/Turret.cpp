#include "Turret.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Controller.hpp"
#include "Game.hpp"
#include "BlueprintLoader.hpp"


void TurretData::loadJson(const Json::Value& root)
{
	GETJSON(controlGroup);

	if(!root["startWep"].isNull())
		startWep = game.getUniverse().getBlueprints().getWeaponSPtr(root["startWep"].asString());

	ShipModuleData::loadJson(root);
}
Turret::Turret(const TurretData& rData) : ShipModule(rData)
{
	if(rData.startWep)
		setWep(rData.startWep);
	m_lastAngle = 0;

	m_controlGroup = rData.controlGroup;
}
Turret::~Turret()
{

}
void Turret::prePhysUpdate()
{
	if(isFunctioning())
	{
		m_lastAngle = atan2(m_lastAim.y-m_fix.getCenter().y, m_lastAim.x-m_fix.getCenter().x);
		m_lastAngle -= m_fix.getAngle();
	}
	if(m_spWep)
		m_spWep->prePhysUpdate(m_fix.getCenter(), m_lastAim, m_lastAngle+m_fix.getAngle(), m_fix.getBodyPtr(), m_fix.getAngle());
	ShipModule::prePhysUpdate();
}
void Turret::postPhysUpdate()
{
	if(isFunctioning())
	{
		m_lastAngle = atan2(m_lastAim.y-m_fix.getCenter().y, m_lastAim.x-m_fix.getCenter().x);
		m_lastAngle -= m_fix.getAngle();
	}
	if(m_spWep)
		m_spWep->postPhysUpdate(m_fix.getCenter(), m_lastAim, m_lastAngle+m_fix.getAngle(), m_fix.getBodyPtr(), m_fix.getAngle());
	ShipModule::postPhysUpdate();
}
void Turret::directive(const CommandInfo& commands)
{
	Map<Directive, bool> rIssues = commands.directives;
	Map<int, bool> controlGroups = commands.weaponGroups;

	if(rIssues[Directive::FirePrimary] && controlGroups[m_controlGroup])
		if(m_spWep && isFunctioning())//if we have a weapon
			if(m_spWep->fire(m_fix, m_pEnergyPool, m_pBallisticPool, m_pMissilePool))//if we successfully fired
			{
				//m_parentChunk->increaseScore();	
			}
}
void Turret::setWep(sptr<const WeaponData> spWep)
{
	assert(spWep.get());
	m_spWep.reset(spWep->generate());
	Team team = m_parentChunk->getBodyComponent().getTeam();
	m_spWep->setTeam(team);
}
void Turret::removeWep()
{
	m_spWep.reset();
}
void Turret::setAim(const b2Vec2& rTarget)
{
	m_lastAim = rTarget;
}
void Turret::toggleStealth(bool toggle)
{
	ShipModule::toggleStealth(toggle);
	if (toggle)
		m_spWep->getDecor()->setAlpha(alpha_stealth_on);
	else 
		m_spWep->getDecor()->setAlpha(alpha_stealth_off);
}


