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
	m_currentBehavior = 2;
	m_numBehaviors = 2;
	m_pCurrentTarget = NULL;
	m_pCurrentDest = NULL;
	
	m_huntingTimer.setCountDown(Rand::get(15.f,30.f));
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

}
void ShipAI::updateDecision()
{
	Controller* cont = game.getUniverse().getControllerFactory().getController(m_controller);
	if(cont == NULL)
		return;
	Controller& rController = *cont;

	b2Body* pBody = rController.getBodyPtr();
	//set controller to be local
	if (!rController.isLocal()){
		rController.toggleLocal(true);
	}
	//default all actions to false
	for (auto it = m_directives.begin(); it != m_directives.end(); ++it)
		it->second = false;

	//check if stuck
	if ( isStuck(pBody->GetPosition()) ){
		m_currentBehavior = 2;
		m_unstuckTimer.restartCountDown();
	}
	//update directives based on current behavior
	if (m_currentBehavior == 0) // we should attack enemy players
	{
		//check for a new target
		if (m_targetTimer.isTimeUp()) {
			m_targetTimer.restartCountDown();
			Team teams[] = {Team::One, Team::Two, Team::Three, Team::Four};
			std::list<Team> teamList(teams, teams + 4);
			teamList.remove(getTeam());
			m_pCurrentTarget = game.getUniverse().getNearestChunkOnTeam(pBody->GetPosition(), rController.getSlave(), teamList);
		}
		//fire at and fly toward target if we have one
		if (m_pCurrentTarget != NULL)
		{
			Vec2 targetPos = m_pCurrentTarget->getBodyPtr()->GetPosition();
			rController.setAim(targetPos);

			flyTowardsChunk(m_pCurrentTarget);
			fireAtTarget();
			m_directives[Directive::Up] = true;
		}
		//check if we have been hunting other players for long enough
		if (m_huntingTimer.isTimeUp())
		{
			int new_behavior = rand() % m_numBehaviors;
			Print << new_behavior;
			if (new_behavior != m_currentBehavior)
			{
				m_currentBehavior = new_behavior;
				m_pCurrentTarget = NULL;
			}
			m_huntingTimer.restartCountDown();
		}


	}
	else if (m_currentBehavior == 1) //find station to capture
	{
		if (m_pCurrentDest == NULL) {
			m_pCurrentDest = game.getUniverse().getNearestStation(pBody->GetPosition(), getTeam());
		}

		if (m_pCurrentDest != NULL)
		{
			flyTowardsChunk(m_pCurrentDest);
			m_directives[Directive::Up] = true;

			//check if capture area has been captured by this AI or our team
			CaptureArea* m_currentCapArea = dynamic_cast<CaptureArea*>((&*m_pCurrentDest->getModuleList()[0]));
			if (m_currentCapArea!= NULL && m_currentCapArea->getProgress() >= 1.0f)
			{
				m_pCurrentTarget = NULL;
				m_pCurrentDest = NULL;
				m_currentBehavior = rand() % m_numBehaviors;
			}

			//check for enemies along the way
			if (m_targetTimer.isTimeUp()) {
				m_targetTimer.restartCountDown();
				Team teams[] = { Team::One, Team::Two, Team::Three, Team::Four };
				std::list<Team> teamList(teams, teams + 4);
				teamList.remove(getTeam());
				m_pCurrentTarget = game.getUniverse().getNearestChunkOnTeam(pBody->GetPosition(), rController.getSlave(), teamList);
			}

			//fire at enemies along the way, but don't fly toward them
			if (m_pCurrentTarget != NULL)
			{
				Vec2 targetPos = m_pCurrentTarget->getBodyPtr()->GetPosition();
				rController.setAim(targetPos);

				fireAtTarget();
			}
		}

	}
	else if (m_currentBehavior == 2) //we are stuck
	{
		//go in reverse for the duration of m_unstuckTimer
		m_directives[Directive::Down] = true;
		if (m_unstuckTimer.isTimeUp()){
			m_pCurrentTarget = NULL;
			m_pCurrentDest = NULL;
			m_currentBehavior = rand() % m_numBehaviors;
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

	b2Body* pBody = rController.getBodyPtr();

	float ourAngle = leon::normRad(pBody->GetAngle() + pi/2);
	Vec2 ourPos = pBody->GetPosition();
	Vec2 targetPos = target->getBodyComponent().getPosition();

	Vec2 diff = targetPos - ourPos;

	float dist = diff.len();

	float targetAngle = leon::normRad(atan2(diff.y, diff.x));

	float diffAngle = leon::normRad(targetAngle - ourAngle);

	if (dist > 15 || m_currentBehavior == 1)
	{
		//angle toward target
		if (diffAngle < pi)
		{
			m_directives[Directive::RollCCW] = true;
		}
		else
			m_directives[Directive::RollCW] = true;
	}
	else 
	{
		//find new perpendicular angle to avoid collision
		Vec2 perpVec(0,0);
		if (diffAngle < pi)
		{
			perpVec = Vec2(diff.y, -diff.x);
		}
		else
		{
			perpVec = Vec2(-diff.y, diff.x);
		}

		targetAngle = leon::normRad(atan2(perpVec.y, perpVec.x));
		diffAngle = leon::normRad(targetAngle - ourAngle);
		if (diffAngle < pi)
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
	Controller* cont = game.getUniverse().getControllerFactory().getController(m_controller);
	if(cont == NULL)
		return;
	Controller& rController = *cont;

	b2Body* pBody = rController.getBodyPtr();

	Vec2 ourPos = pBody->GetPosition();
	Vec2 targetPos = m_pCurrentTarget->getBodyComponent().getPosition();

	Vec2 diff = targetPos - ourPos;

	if(diff.len() < 40)
	{
		m_directives[Directive::FirePrimary] = true;
	}


}

bool ShipAI::isStuck(Vec2 curPos)
{
	if (m_stuckTimer.isTimeUp()){
		m_stuckTimer.restartCountDown();
		if (curPos == oldPos){
			return true;
		}
		oldPos = curPos;
	}
	return false;
}


