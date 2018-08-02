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
	m_laneUpdateTimer.setCountDown(0.2f);
	setController(controller_index);
	setTeam(team);
	m_targetAquireRange = 20;
	m_lane = Lane::Middle;

	for(int i = 1; i <= 9; ++i)
		m_weaponGroups[i] = true;
}
ShipAI::~ShipAI()
{

}
void ShipAI::onShipDestroyed()
{
	Controller* cont = getGame()->getUniverse().getControllerFactory().getController(m_controller);
}
void ShipAI::updateDecision()
{
	//default all actions to false
	for(auto it = m_directives.begin(); it != m_directives.end(); ++it)
		it->second = false;

	Controller* cont = getGame()->getUniverse().getControllerFactory().getController(m_controller);
	if(cont == nullptr)
		return;
	
	Controller& rController = *cont;
	Chunk* chunk = rController.getChunk();

	//set controller to be local
	if(!rController.isLocal())
	{
		rController.toggleLocal(true);
	}
	if(m_weaponRange == 0.f)
	{
		m_weaponRange = chunk->maxWeaponRange();
	}
	
	auto& body = chunk->getBodyComponent();
	auto ourPos = body.getPosition();


	{
		tryGetNewClosestTarget(ourPos, chunk);
		//fire at and fly toward target if we have one
		if(m_pCurrentTarget != nullptr)
		{
			Vec2 targetPos = m_pCurrentTarget->getBodyComponent().getPosition();

			//aim at target. TODO, lead target
			rController.setAim(targetPos);

			//fire or fly at target
			const float distToTarget = ourPos.to(targetPos).len();
			if(distToTarget < m_weaponRange)
				fireAtTarget();
			else

				flyToTarget(targetPos);
		}
		else
		{
			if(m_laneUpdateTimer.isTimeUp())
			{
				m_laneUpdateTimer.restartCountDown();
				m_laneDestination = getLaneTarget(ourPos);
			}

			flyToTarget(m_laneDestination);
		}
	}

	//send commands to controller to move the ship
	CommandInfo commands;
	commands.isLocal = true;
	commands.directives = m_directives;
	commands.weaponGroups = m_weaponGroups;
	rController.locallyUpdate(commands);
}
Vec2 ShipAI::getLaneTarget(const Vec2& ourPos) const
{
	return getGame()->getUniverse().getLaneTarget(getTeam(), m_lane, ourPos);
}
void ShipAI::tryGetNewClosestTarget(const Vec2& ourPos, const Chunk* chunk)
{
	//check for a new target
	if(m_targetTimer.isTimeUp())
	{
		m_targetTimer.restartCountDown();
		Team teams[] = { Team::One, Team::Two, Team::Three, Team::Four };
		std::list<Team> teamList(teams, teams + 4);
		teamList.remove(getTeam());
		m_pCurrentTarget = getGame()->getUniverse().getNearestChunk(ourPos, chunk, teamList);
		if(auto target = m_pCurrentTarget)
		{
			const auto targetPos = target->getBodyComponent().getPosition();
			const auto distToTarget = targetPos.to(ourPos).len();
			if(distToTarget > m_targetAquireRange)
			{
				m_pCurrentTarget = nullptr;
			}
		}
		else
			m_pCurrentTarget = nullptr;
	}
}
void ShipAI::flyToTarget(Vec2 pos)
{
	angleToTarget(pos);
	m_directives[Directive::Up] = true;
}
void ShipAI::angleToTarget(Vec2 target)
{
	Controller* cont = getGame()->getUniverse().getControllerFactory().getController(m_controller);
	if(cont == nullptr)
	{
		WARNING;
		return;
	}

	Controller& controller = *cont;
	Chunk* chunk = controller.getChunk();
	auto& body = chunk->getBodyComponent();

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


