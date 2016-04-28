#pragma once

#include "BasePlayerTraits.hpp"
#include "NonCopyable.hpp"
#include "Timer.hpp"

class Chunk;
enum class Directive;

class ShipAI : public BasePlayerTraits, NonCopyable
{
public:
	ShipAI(int team, int controller_index);
	~ShipAI();

	void updateDecision();

protected:
private:
	bool isStuck(b2Vec2 curPos);//check if we are stuck
	Timer m_stuckTimer; //how often we check our position to see if we are stuck
	Timer m_unstuckTimer; //how long we go in reverse to get unstuck
	b2Vec2 oldPos;

	Timer m_huntingTimer; //timer for how long we should hunt enemies for
	Timer m_targetTimer; //timer for switching to a closer enemy
	void flyTowardsChunk(Chunk* target);//fly ship towards chunk
	void fireAtTarget();//fire at m_pCurrentTarget


	int m_currentBehavior;//our current behavior from 0 to m_numBehaviors
	int m_numBehaviors;//number of behaviors, includes stuck behavior which should always be last
	Chunk* m_pCurrentTarget;//our current enemy target nearest to us
	Chunk* m_pCurrentDest;//our current destination to fly to
	
	Map<Directive, bool> m_directives;
	Map<int, bool> m_weaponGroups;
};

