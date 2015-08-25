#include "Thruster.hpp"
#include "Controller.hpp"

using namespace std;

Thruster::Thruster(const ThrusterData& rData) : ShipModule(rData)
{
	m_eConsump = rData.energyConsumption;
	m_force = rData.force;
	m_torque = rData.torque;

	m_forceVec = b2Vec2(0,1);
	m_isCCW = true;
}
Thruster::~Thruster()
{

}
void Thruster::prePhysUpdate()
{
	ShipModule::prePhysUpdate();
}
void Thruster::postPhysUpdate()
{
	ShipModule::postPhysUpdate();
}
void Thruster::directive(std::map<Directive, bool>& rIssues)
{
	if(rIssues[Directive::Up])
		thrust(b2Vec2(0,1));
	if(rIssues[Directive::Down])
		thrust(b2Vec2(0,-1));
	if(rIssues[Directive::RollCCW])
		torque(true);
	if(rIssues[Directive::RollCW])
		torque(false);

	//Evan - added boost mechanic
	if (rIssues[Directive::Boost])
		thrust(b2Vec2(0, 2));
}
void Thruster::thrust(const b2Vec2& rDirection)
{
	if(functioning())
	{
		float eThisStep = m_eConsump*game.getUniverse().getTimeStep();

		if(eThisStep <= m_pEnergyPool->getValue())
		{
			m_pEnergyPool->changeValue(-eThisStep);

			float angle = m_fix.getAngle();

			b2Vec2 forceVec;
			forceVec.x = cos(-angle)*rDirection.x + sin(-angle)*rDirection.y;//negative because THAT IS CORRECT, go lookup the equation!
			forceVec.y = -sin(-angle)*rDirection.x + cos(-angle)*rDirection.y;

			forceVec.x *= m_force;
			forceVec.y *= m_force;

			m_fix.applyForce(forceVec);
		}
	}
}
void Thruster::torque(bool CCW)
{
	if(functioning())
	{
		float eThisStep = m_eConsump*game.getUniverse().getTimeStep();

		if(eThisStep <= m_pEnergyPool->getValue())
		{
			m_pEnergyPool->changeValue(-eThisStep);
			if(CCW)
				m_fix.applyTorque(m_torque);
			else
				m_fix.applyTorque(-m_torque);
		}
	}
}
