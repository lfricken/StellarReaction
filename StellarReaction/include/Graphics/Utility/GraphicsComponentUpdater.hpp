#ifndef GRAPHICSCOMPONENTUPDATER_HPP
#define GRAPHICSCOMPONENTUPDATER_HPP

#include "stdafx.hpp"

class GraphicsComponent;

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
