#ifndef GRAPHICSCOMPONENTUPDATER_HPP
#define GRAPHICSCOMPONENTUPDATER_HPP

#include "stdafx.hpp"

class GraphicsComponent;

/// <summary>
/// Holds a reference to all Graphics Components so we can tell them to update their animation.
/// </summary>
class GraphicsComponentUpdater
{
public:
	GraphicsComponentUpdater();
	virtual ~GraphicsComponentUpdater();

	void store(GraphicsComponent* pTarget);
	void free(GraphicsComponent* pTarget);

	void update();

protected:
private:
	std::vector<GraphicsComponent*> m_targets;
};

#endif // GRAPHICSCOMPONENTUPDATER_HPP
