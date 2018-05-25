#include "Turret.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Controller.hpp"
#include "Game.hpp"
#include "BlueprintLoader.hpp"
#include "ModuleParent.hpp"


void TurretData::setTeam(Team team)
{
	ShipModuleData::setTeam(team);
	startWep = this->title;
}
void TurretData::loadJson(const Json::Value& root)
{
	GETJSON(controlGroup);
	//GETJSON(startWep); this is computed from title now in setTeam!

	ShipModuleData::loadJson(root);
}
Turret::Turret(const TurretData& rData) : ShipModule(rData)
{
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
		m_spWep->prePhysUpdate(m_fix.getCenter(), m_lastAim, m_lastAngle+m_fix.getAngle(), m_fix.getParentBody(), m_fix.getAngle());
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
		m_spWep->postPhysUpdate(m_fix.getCenter(), m_lastAim, m_lastAngle + m_fix.getAngle(), m_fix.getParentBody(), m_fix.getAngle());
	ShipModule::postPhysUpdate();
}
void Turret::directive(const CommandInfo& commands)
{
	//Map<int, bool> controlGroups = commands.weaponGroups;
	auto it = commands.directives.find(Directive::FirePrimary);
	if(it != commands.directives.cend() && it->second)
		if(m_spWep && isFunctioning())//if we have a weapon
			if(m_spWep->fire(m_fix, rangeModifiers.ranges))//if we successfully fired
			{
				//m_parentChunk->increaseScore();	
			}
}
void Turret::setWep(String wepName)
{
	auto wep = game.getUniverse().getBlueprints().getWeaponSPtr(wepName);
	m_spWep.reset(wep->generate());
	Team team = m_parent->getBodyComponent().getTeam();
	m_spWep->setTeam(team);
}
void Turret::removeWep()
{
	m_spWep.reset();
}

void Turret::setAim(const Vec2& rTarget)
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
const Weapon* Turret::getWeapon()
{
	return m_spWep.get();
}


