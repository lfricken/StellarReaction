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
	bool isStuck(b2Vec2 curPos);
	Timer m_stuckTimer;
	Timer m_unstuckTimer;
	b2Vec2 oldPos;

	Timer m_targetTimer;
	void flyTowardsTarget();
	void fireAtTarget();


	int m_currentBehavior;
	Chunk* m_pCurrentTarget;
	Map<Directive, bool> m_directives;
	Map<int, bool> m_weaponGroups;
};

