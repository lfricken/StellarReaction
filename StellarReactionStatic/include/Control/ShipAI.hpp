#pragma once

#include "BasePlayerTraits.hpp"
#include "NonCopyable.hpp"
#include "Timer.hpp"

class Chunk;
enum class Directive;

/// \brief An AI designed to control a ship.
///
/// A list of these are stored on the host, which send commands to Controllers.
/// These controllers are synced between each Game.
class ShipAI : public BasePlayerTraits, NonCopyable
{
public:
	ShipAI(Team team, int controller_index);
	~ShipAI();

	/// Update what the AI is trying to do.
	void updateDecision();

protected:
	void onShipDestroyed();

private:
	bool isStuck(Vec2 curPos);//check if we are stuck
	Timer m_stuckTimer; //how often we check our position to see if we are stuck
	Timer m_unstuckTimer; //how long we go in reverse to get unstuck
	Vec2 oldPos;

	Timer m_huntingTimer; //timer for how long we should hunt enemies for
	Timer m_targetTimer; //timer for switching to a closer enemy
	void flyTowardsChunk(Chunk* target);//fly ship towards chunk
	void fireAtTarget();//fire at m_pCurrentTarget


	int m_currentBehavior;//our current behavior from 0 to m_numBehaviors
	int m_numBehaviors;//number of behaviors, includes stuck behavior which should always be last
	wptr<Chunk> m_pCurrentTarget;//our current enemy target nearest to us
	wptr<Chunk> m_pCurrentDest;//our current destination to fly to
	
	Map<Directive, bool> m_directives;
	Map<int, bool> m_weaponGroups;
};

