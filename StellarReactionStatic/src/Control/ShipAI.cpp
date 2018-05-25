#include "ShipAI.hpp"
#include "Globals.hpp"
#include "Universe.hpp"
#include "Controller.hpp"
#include "Chunk.hpp"
#include "Convert.hpp"
#include "CommandInfo.hpp"
#include "CaptureArea.hpp"

ShipAI::ShipAI(Team team, int controller_index) : BasePlayerTraits("ai")
{
	m_weaponRange = 0.f;
	m_huntingTimer.setCountDown(Rand::get(15.f, 30.f));
	m_targetTimer.setCountDown(1.f);
	m_stuckTimer.setCountDown(2.f);
	m_unstuckTimer.setCountDown(1.f);
	setController(controller_index);
	setTeam(team);
	for(int i = 1; i <= 9; ++i)
		m_weaponGroups[i] = true;
}
ShipAI::~ShipAI()
{

}
void ShipAI::onShipDestroyed()
{
	Controller* cont = game.getUniverse().getControllerFactory().getController(m_controller);
}
void ShipAI::updateDecision()
{
	Controller* cont = game.getUniverse().getControllerFactory().getController(m_controller);
	if(cont == nullptr)
		return;
	
	Controller& rController = *cont;
	
	auto& body = rController.getChunk()->getBodyComponent();
	auto ourPos = body.getPosition();

	//set controller to be local
	if(!rController.isLocal())
	{
		rController.toggleLocal(true);
	}
	//default all actions to false
	for(auto it = m_directives.begin(); it != m_directives.end(); ++it)
		it->second = false;

	{
		//check for a new target
		if(m_targetTimer.isTimeUp())
		{
			m_targetTimer.restartCountDown();
			Team teams[] = { Team::One, Team::Two, Team::Three, Team::Four };
			std::list<Team> teamList(teams, teams + 4);
			teamList.remove(getTeam());
			m_pCurrentTarget = game.getUniverse().getNearestChunk(ourPos, rController.getChunk().get(), teamList);
		}
		//fire at and fly toward target if we have one
		if(auto target = m_pCurrentTarget.lock())
		{
			Vec2 targetPos = target->getBodyComponent().getPosition();

			//TODO, lead target
			rController.setAim(targetPos);

			angleToTarget(targetPos);
			
			if(m_weaponRange == 0.f)
			{
				m_weaponRange = rController.getChunk()->maxWeaponRange();
			}
			const float len = ourPos.to(targetPos).len();
			if(len < m_weaponRange)
				fireAtTarget();
			else
				m_directives[Directive::Up] = true;
		}
	}

	//send commands to controller to move the ship
	CommandInfo commands;
	commands.isLocal = true;
	commands.directives = m_directives;
	commands.weaponGroups = m_weaponGroups;
	rController.locallyUpdate(commands);
}
void ShipAI::angleToTarget(Vec2 target)
{
	Controller* cont = game.getUniverse().getControllerFactory().getController(m_controller);
	if(cont == nullptr)
	{
		WARNING;
		return;
	}

	Controller& rController = *cont;
	auto& body = rController.getChunk()->getBodyComponent();

	float ourAngle = Convert::normRad(body.getAngle() + Math::Tau / 4);
	Vec2 ourPos = body.getPosition();

	Vec2 diff = target - ourPos;

	float targetAngle = Convert::normRad(atan2(diff.y, diff.x));
	float diffAngle = Convert::normRad(targetAngle - ourAngle);

	//angle toward target
	if(diffAngle < Math::Pi)
		m_directives[Directive::RollCCW] = true;
	else
		m_directives[Directive::RollCW] = true;
}
void ShipAI::fireAtTarget()
{
	m_directives[Directive::FirePrimary] = true;
}
bool ShipAI::isStuck(Vec2 curPos)
{
	if(m_stuckTimer.isTimeUp())
	{
		m_stuckTimer.restartCountDown();
		if(curPos == oldPos)
		{
			return true;
		}
		oldPos = curPos;
	}
	return false;
}


