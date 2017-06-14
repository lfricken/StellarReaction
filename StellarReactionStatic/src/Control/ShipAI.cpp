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
	m_currentBehavior = 0;
	m_numBehaviors = 1;

	m_huntingTimer.setCountDown(Rand::get(15.f, 30.f));
	m_targetTimer.setCountDown(5.f);
	m_stuckTimer.setCountDown(2.f);
	m_unstuckTimer.setCountDown(1.f);
	setController(controller_index);
	setTeam(team);
	for(int i = 1; i <= 9; ++i)
		m_weaponGroups[i] = true;
}

ShipAI::~ShipAI()
{
	//dout << "\ndead ship ai.";
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

	//if(pBody == nullptr)//detect that ship was destroyed
	//{
	//	onShipDestroyed();
	//	return;
	//}

	//set controller to be local
	if(!rController.isLocal())
	{
		rController.toggleLocal(true);
	}
	//default all actions to false
	for(auto it = m_directives.begin(); it != m_directives.end(); ++it)
		it->second = false;

	//check if stuck
	if(isStuck(body.getPosition()))
	{
		m_currentBehavior = 2;
		m_unstuckTimer.restartCountDown();
	}
	//update directives based on current behavior
	//if (m_currentBehavior == 0) // we should attack enemy players
	{
		//check for a new target
		if(m_targetTimer.isTimeUp())
		{
			m_targetTimer.restartCountDown();
			Team teams[] = { Team::One, Team::Two, Team::Three, Team::Four };
			std::list<Team> teamList(teams, teams + 4);
			teamList.remove(getTeam());
			m_pCurrentTarget = game.getUniverse().getNearestChunk(body.getPosition(), rController.getChunk().get(), teamList);
		}
		//fire at and fly toward target if we have one
		if(auto target = m_pCurrentTarget.lock())
		{
			Vec2 targetPos = target->getBodyPtr()->GetPosition();

			//TODO, lead target
			rController.setAim(targetPos);

			flyTowardsChunk(target.get());
			fireAtTarget();
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
void ShipAI::flyTowardsChunk(Chunk* target)
{
	Controller* cont = game.getUniverse().getControllerFactory().getController(m_controller);
	if(cont == NULL)
		return;
	Controller& rController = *cont;

	auto& body = rController.getChunk()->getBodyComponent();

	float ourAngle = leon::normRad(body.getAngle() + pi / 2);
	Vec2 ourPos = body.getPosition();
	Vec2 targetPos = target->getBodyComponent().getPosition();

	Vec2 diff = targetPos - ourPos;

	float dist = diff.len();

	float targetAngle = leon::normRad(atan2(diff.y, diff.x));

	float diffAngle = leon::normRad(targetAngle - ourAngle);

	if(dist > 15 || m_currentBehavior == 1)
	{
		//angle toward target
		if(diffAngle < pi)
		{
			m_directives[Directive::RollCCW] = true;
		}
		else
			m_directives[Directive::RollCW] = true;
	}
	else
	{
		//find new perpendicular angle to avoid collision
		Vec2 perpVec(0, 0);
		if(diffAngle < pi)
		{
			perpVec = Vec2(diff.y, -diff.x);
		}
		else
		{
			perpVec = Vec2(-diff.y, diff.x);
		}

		targetAngle = leon::normRad(atan2(perpVec.y, perpVec.x));
		diffAngle = leon::normRad(targetAngle - ourAngle);
		if(diffAngle < pi)
		{
			m_directives[Directive::RollCCW] = true;
		}
		else
		{
			m_directives[Directive::RollCW] = true;
		}
	}

}
void ShipAI::fireAtTarget()
{
	if(auto target = m_pCurrentTarget.lock())
	{
		Controller* cont = game.getUniverse().getControllerFactory().getController(m_controller);
		if(cont == NULL)
			return;
		Controller& rController = *cont;

		auto& body = rController.getChunk()->getBodyComponent();

		Vec2 ourPos = body.getPosition();
		Vec2 targetPos = target->getBodyComponent().getPosition();

		Vec2 diff = targetPos - ourPos;

		if(diff.len() < 40)
		{
			m_directives[Directive::FirePrimary] = true;
		}

	}
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


