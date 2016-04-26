#include "ShipAI.hpp"
#include "Globals.hpp"
#include "Universe.hpp"
#include "Controller.hpp"
#include "Chunk.hpp"
#include "Convert.hpp"
#include "CommandInfo.hpp"

using namespace std;

ShipAI::ShipAI(int team, int controller_index) : BasePlayerTraits("ai")
{
	m_currentBehavior = 2;
	m_pCurrentTarget = NULL;
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
	Controller& rController = game.getUniverse().getControllerFactory().getController(m_controller);
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
		m_currentBehavior = 1;
		m_unstuckTimer.restartCountDown();
	}
	//update directives based on current behavior
	if (m_currentBehavior == 0) // we should attack enemy players
	{
		if (m_targetTimer.isTimeUp() || m_pCurrentTarget == NULL) {
			m_targetTimer.restartCountDown();
			int teams[] = {1,2,3,4};
			std::list<int> teamList(teams, teams + 4);
			teamList.remove(getTeam());
			m_pCurrentTarget = game.getUniverse().getNearestChunkOnTeam(pBody->GetPosition(), rController.getSlave(), teamList);
		}


		if (m_pCurrentTarget != NULL)
		{
			b2Vec2 targetPos = m_pCurrentTarget->getBodyPtr()->GetPosition();
			rController.setAim(targetPos);

			flyTowardsTarget();
			fireAtTarget();
			m_directives[Directive::Up] = true;
		}

	}
	else if (m_currentBehavior == 1) //we are stuck
	{
		m_directives[Directive::Down] = true;
		if (m_unstuckTimer.isTimeUp()){
			m_currentBehavior = 0;
		}
	}
	else if (m_currentBehavior == 2) //find station to capture
	{
		if (m_pCurrentTarget == NULL) {
			m_pCurrentTarget = game.getUniverse().getNearestStation(pBody->GetPosition(), getTeam());
		}

		if (m_pCurrentTarget != NULL)
		{
			flyTowardsTarget();
			m_directives[Directive::Up] = true;
		}
	}

	CommandInfo commands;
	commands.isLocal = true;
	commands.directives = m_directives;
	commands.weaponGroups = m_weaponGroups;

	rController.locallyUpdate(commands);
}
void ShipAI::flyTowardsTarget()
{
	Controller& rController = game.getUniverse().getControllerFactory().getController(m_controller);
	b2Body* pBody = rController.getBodyPtr();

	float ourAngle = leon::normRad(pBody->GetAngle() + pi/2);
	b2Vec2 ourPos = pBody->GetPosition();
	b2Vec2 targetPos = m_pCurrentTarget->getBodyComponent().getPosition();

	b2Vec2 diff = targetPos - ourPos;

	float dist = diff.Length();

	float targetAngle = leon::normRad(atan2(diff.y, diff.x));

	float diffAngle = leon::normRad(targetAngle - ourAngle);

	if (dist > 15 || m_currentBehavior == 2)
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
		b2Vec2 perpVec = b2Vec2_zero;
		if (diffAngle < pi)
		{
			perpVec = b2Vec2(diff.y, -diff.x);
		}
		else
			perpVec = b2Vec2(-diff.y, diff.x);
		targetAngle = leon::normRad(atan2(perpVec.y, perpVec.x));
		diffAngle = leon::normRad(targetAngle - ourAngle);
		if (diffAngle < pi)
		{
			m_directives[Directive::RollCCW] = true;
		}
		else
			m_directives[Directive::RollCW] = true;
	}

}
void ShipAI::fireAtTarget()
{
	Controller& rController = game.getUniverse().getControllerFactory().getController(m_controller);
	b2Body* pBody = rController.getBodyPtr();

	b2Vec2 ourPos = pBody->GetPosition();
	b2Vec2 targetPos = m_pCurrentTarget->getBodyComponent().getPosition();

	b2Vec2 diff = targetPos - ourPos;

	if(diff.Length() < 40)
	{
		m_directives[Directive::FirePrimary] = true;
	}


}

bool ShipAI::isStuck(b2Vec2 curPos)
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


