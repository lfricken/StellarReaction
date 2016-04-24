#include "ShipAI.hpp"
#include "Globals.hpp"
#include "Universe.hpp"
#include "Controller.hpp"
#include "Chunk.hpp"
#include "Convert.hpp"
#include "CommandInfo.hpp"

using namespace std;

ShipAI::ShipAI() : BasePlayerTraits("ai")
{
	m_pCurrentTarget = NULL;
	for(int i = 1; i <= 9; ++i)
		m_weaponGroups[i] = true;
}
ShipAI::~ShipAI()
{

}
void ShipAI::updateDecision()
{
	//default to false
	for(auto it = m_directives.begin(); it != m_directives.end(); ++it)
		it->second = false;

	Controller& rController = game.getUniverse().getControllerFactory().getController(m_controller);
	b2Body* pBody = rController.getBodyPtr();


	if (m_pCurrentTarget == NULL) {
		m_pCurrentTarget = game.getUniverse().getNearestChunk(pBody->GetPosition(), rController.getSlave());
		cout << "\n" << rController.getSlaveName() << " " << m_controller;
	}

	
	if(m_pCurrentTarget != NULL)
	{
		b2Vec2 targetPos = m_pCurrentTarget->getBodyPtr()->GetPosition();
		rController.setAim(targetPos);

		flyTowardsTarget();
		fireAtTarget();
	}


	m_directives[Directive::Up] = true;

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

	float targetAngle = leon::normRad(atan2(diff.y, diff.x));

	float diffAngle = leon::normRad(targetAngle - ourAngle);


	if(diffAngle < pi)
	{
		m_directives[Directive::RollCCW] = true;
	}
	else
		m_directives[Directive::RollCW] = true;

}
void ShipAI::fireAtTarget()
{
	Controller& rController = game.getUniverse().getControllerFactory().getController(m_controller);
	b2Body* pBody = rController.getBodyPtr();

	b2Vec2 ourPos = pBody->GetPosition();
	b2Vec2 targetPos = m_pCurrentTarget->getBodyComponent().getPosition();

	b2Vec2 diff = targetPos - ourPos;

	if(diff.Length() < 45)
	{
		m_directives[Directive::FirePrimary] = true;
	}


}


