#pragma once

#include "BasePlayerTraits.hpp"

class Chunk;
enum class Directive;

class ShipAI : public BasePlayerTraits
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
	std::map<Directive, bool> m_directives;

};

