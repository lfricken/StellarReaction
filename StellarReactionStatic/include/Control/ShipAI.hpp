#pragma once

#include "BasePlayerTraits.hpp"
#include "NonCopyable.hpp"

class Chunk;
enum class Directive;

/// An AI designed to control a ship.
/// A list of these are stored on the host, which send commands to Controllers.
/// These controllers are synced between each Game.
class ShipAI : public BasePlayerTraits, NonCopyable
{
public:
	ShipAI();
	~ShipAI();

	/// Update what the AI is trying to do.
	void updateDecision();
	/// Called when we want to fly towards our target.
	void flyTowardsTarget();
	/// Called when we want to fire at our target.
	void fireAtTarget();

protected:
private:

	Chunk* m_pCurrentTarget;
	Map<Directive, bool> m_directives;
	Map<int, bool> m_weaponGroups;
};

