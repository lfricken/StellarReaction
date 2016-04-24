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
	m_currentBehavior = 0;
	m_pCurrentTarget = NULL;
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
	if (m_currentBehavior == 0) // we should attack enemy players
	{
		//default to false
		for (auto it = m_directives.begin(); it != m_directives.end(); ++it)
			it->second = false;

		Controller& rController = game.getUniverse().getControllerFactory().getController(m_controller);
		b2Body* pBody = rController.getBodyPtr();

		if (!rController.isLocal()){
			rController.toggleLocal(true);
		}


		if (m_pCurrentTarget == NULL) {
			m_pCurrentTarget = game.getUniverse().getNearestChunk(pBody->GetPosition(), rController.getSlave());
		}


		if (m_pCurrentTarget != NULL)
		{
			b2Vec2 targetPos = m_pCurrentTarget->getBodyPtr()->GetPosition();
			rController.setAim(targetPos);

			flyTowardsTarget();
			fireAtTarget();
		}
		m_directives[Directive::Up] = true;

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

	if (dist > 15)
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


