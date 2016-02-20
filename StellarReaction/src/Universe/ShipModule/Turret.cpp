#include "Turret.hpp"

#include "Controller.hpp"

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
	std::cout<<"Active Control Group: " << game.getLocalPlayer().getActiveControlGroup() << "\n Turret Control Group: " << m_controlGroup << std::endl;
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
void Turret::stealthOn(bool toggle)
{
	ShipModule::stealthOn(toggle);
	if (toggle)
		m_spWep->getDecor()->setAlpha(50);
	else 
		m_spWep->getDecor()->setAlpha(255);
}
