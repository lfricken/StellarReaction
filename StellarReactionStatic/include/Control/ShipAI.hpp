#pragma once

#include "BasePlayerTraits.hpp"
#include "NonCopyable.hpp"
#include "Timer.hpp"
#include "Factory.hpp"
#include "Lanes.hpp"

class Chunk;
enum class Directive;

/// \brief An AI designed to control a ship.
///
/// A list of these are stored on the host, which send commands to Controllers.
/// These controllers are synced between each getGame()->
class ShipAI : public BasePlayerTraits, public FactoryObject, NonCopyable
{
public:
	ShipAI(Team team, int controller_index);
	~ShipAI();

	/// Update what the AI is trying to do.
	void updateDecision();

	void flyToTarget(Vec2 pos);

protected:
	void tryGetNewClosestTarget(const Vec2& ourPos, const Chunk* chunk);
	void onShipDestroyed();
	Vec2 getLaneTarget(const Vec2& ourPos) const;

private:

	Lane m_lane;
	float m_targetAquireRange;//how far away will the ai try to grab a target?

	bool isStuck(Vec2 curPos);//check if we are stuck
	Timer m_stuckTimer; //how often we check our position to see if we are stuck
	Timer m_unstuckTimer; //how long we go in reverse to get unstuck
	Vec2 oldPos;

	Timer m_huntingTimer; //timer for how long we should hunt enemies for
	Timer m_targetTimer; //timer for switching to a closer enemy
	void angleToTarget(Vec2 target);//fly ship towards chunk
	void fireAtTarget();//fire at m_pCurrentTarget
	float m_weaponRange;

	Chunk* m_pCurrentTarget;//our current enemy target nearest to us

	Timer m_laneUpdateTimer; // how frequently to grab a new lane.
	Vec2 m_laneDestination; // our current destination to fly to
	
	Map<Directive, bool> m_directives;
	Map<int, bool> m_weaponGroups;
};

