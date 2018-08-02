#pragma once

#include "stdafx.hpp"
#include "NonCopyable.hpp"

class GraphicsComponent;


/// Holds a reference to all GraphicsComponents so we can tell them to update their animation.
class GraphicsComponentUpdater : NonCopyable
{
public:
	GraphicsComponentUpdater();
	virtual ~GraphicsComponentUpdater();

	/// Pass in a pointer to an object that should get updated later.
	void store(GraphicsComponent* pTarget);
	/// Pass in a pointer to an object that should no longer be updated.
	void freeThis(GraphicsComponent* pTarget);
	/// Update all GraphicsComponents.
	void update();

protected:
private:
	List<GraphicsComponent*> m_targets;
};


