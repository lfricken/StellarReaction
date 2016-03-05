#include "Turret.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Controller.hpp"
#include "Game.hpp"
#include "BlueprintLoader.hpp"

Turret::Turret(const TurretData& rData) : ShipModule(rData)
{
	if(!rData.startEmpty)
		setWep(rData.startWep);
	m_lastAngle = 0;
}
Turret::~Turret()
{

}
void Turret::prePhysUpdate()
{
	if(functioning())
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
	if(functioning())
	{
		m_lastAngle = atan2(m_lastAim.y-m_fix.getCenter().y, m_lastAim.x-m_fix.getCenter().x);
		m_lastAngle -= m_fix.getAngle();
	}
	if(m_spWep)
		m_spWep->postPhysUpdate(m_fix.getCenter(), m_lastAim, m_lastAngle+m_fix.getAngle(), m_fix.getBodyPtr(), m_fix.getAngle());
	ShipModule::postPhysUpdate();
}
void Turret::directive(std::map<Directive, bool>& rIssues)
{
	if(rIssues[Directive::FirePrimary] && game.getLocalPlayer().getActiveControlGroup() == m_controlGroup)
		if(m_spWep && functioning())//if we have a weapon
			if(m_spWep->fire(m_fix, m_pEnergyPool, m_pBallisticPool, m_pMissilePool))//if we successfully fired
			{

			}
}
void Turret::setWep(sptr<const WeaponData> spWep)
{
	assert(spWep.get());
	m_spWep.reset(spWep->generate());
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
void TurretData::loadJson(const Json::Value& root)
{
	if(!root["Copies"].isNull())
		*this = *dynamic_cast<const TurretData*>(game.getUniverse().getBlueprints().getChunkSPtr(root["Copies"].asString()).get());

	if(!root["StartEmpty"].isNull())
		startEmpty = root["StartEmpty"].asBool();
	if(!root["Weapon"].isNull())
		startWep = game.getUniverse().getBlueprints().getWeaponSPtr(root["Weapon"]["Title"].asString());

	ShipModuleData::loadJson(root);
}


