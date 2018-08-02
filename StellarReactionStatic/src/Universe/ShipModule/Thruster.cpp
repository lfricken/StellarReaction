#include "Thruster.hpp"
#include "Controller.hpp"



void ThrusterData::loadJson(const Json::Value& root)
{
	GETJSON(force);
	GETJSON(torque);
	GETJSON(energyConsumption);
	GETJSON(boostThrustMult);
	GETJSON(boostCostMult);

	ShipModuleData::loadJson(root);
}
Thruster::Thruster(const ThrusterData& rData) : ShipModule(rData)
{
	m_eConsump = rData.energyConsumption;
	m_force = rData.force;
	m_torque = rData.torque;

	m_boostThrust = rData.boostThrustMult;
	m_boostCostMulti = rData.boostCostMult;

	m_forceVec = Vec2(0, 1);
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
void Thruster::directive(const CommandInfo& commands)
{
	Map<Directive, bool> rIssues = commands.directives;

	if(rIssues[Directive::Up])
	{
		if(rIssues[Directive::Boost])
			thrust(Vec2(0, m_boostThrust));
		else
			thrust(Vec2(0, 1));
	}
	if(rIssues[Directive::Down])
		thrust(Vec2(0, -1));
	if(rIssues[Directive::RollCCW])
		torque(true);
	if(rIssues[Directive::RollCW])
		torque(false);
}
void Thruster::thrust(const Vec2& rDirMultiplier)
{
	float eThisStep = m_eConsump*getGame()->getUniverse().getTimeStep();
	if(rDirMultiplier.len() > 1.0f)//if they are boosting at all
		eThisStep *= m_boostCostMulti;//Boosting costs more.

	if((*ranges)[RangeList::Energy].tryChange(-eThisStep))
	{
		float angle = m_fix.getParentBody()->getAngle();

		Vec2 forceVec = rDirMultiplier.rotate(angle);
		forceVec *= m_force * functionalCapacity();

		m_fix.getParentBody()->applyForce(forceVec);
	}
}
void Thruster::torque(bool CCW)
{
	float eThisStep = m_eConsump*getGame()->getUniverse().getTimeStep();

	if((*ranges)[RangeList::Energy].tryChange(-eThisStep))
	{
		float torque = m_torque * functionalCapacity();

		if(CCW)
			m_fix.getParentBody()->applyTorque(torque);
		else
			m_fix.getParentBody()->applyTorque(-torque);
	}
}

