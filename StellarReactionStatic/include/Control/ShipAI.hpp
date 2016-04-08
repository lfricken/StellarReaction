#pragma once

#include "BasePlayerTraits.hpp"
#include "NonCopyable.hpp"

class Chunk;
enum class Directive;

class ShipAI : public BasePlayerTraits, NonCopyable
{
public:
	ShipAI();
	~ShipAI();

	void updateDecision();
	void flyTowardsTarget();
	void fireAtTarget();

protected:
private:

	Chunk* m_pCurrentTarget;
	Map<Directive, bool> m_directives;
	Map<int, bool> m_weaponGroups;
};

