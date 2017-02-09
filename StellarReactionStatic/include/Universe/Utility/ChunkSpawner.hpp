#pragma once

#include "stdafx.hpp"
#include "Timer.hpp"
#include "IOComponent.hpp"
#include "Team.hpp"

class Universe;

/// Used briefly to create a collection of Hazards in the world.
class ChunkSpawner
{
public:
	ChunkSpawner(Universe* universe, const Json::Value& root);
	///Spawns a hazard in a random location within the field.
	void update();
	///Recieve commands.
	void input(String command, sf::Packet data);
private:
	void resetCountdown();
	Vec2 pickRandPoint();

	Universe* m_pUniverse;

	/// <summary>
	/// The team of the chunks this spawner creates.
	/// </summary>
	Team m_team;
	/// <summary>
	/// True if AI are created with each chunk.
	/// </summary>
	bool m_makesAI;
	/// <summary>
	/// Should we be trying to spawn things now.
	/// </summary>
	bool m_isEnabled;
	/// <summary>
	/// Blueprints to randomly choose from.
	/// </summary>
	List<String> m_blueprints;
	/// <summary>
	/// Spawn limit. Can only spawn this many per game. -1 is unlimited.
	/// </summary>
	int m_maxNumber;
	/// <summary>
	/// How often an enemy spawns.
	/// </summary>
	float m_spawnPeriod;
	/// <summary>
	/// Max value added or subtracted from the spawn period per spawn.
	/// </summary>
	float m_spawnPeriodVariance;
	/// <summary>
	/// Radius to choose a random point.
	/// </summary>
	float m_radius;
	/// <summary>
	/// Center of the spawning positions.
	/// </summary>
	Vec2 m_origin;
	/// <summary>
	/// Keep track of time to spawn enemies from it.
	/// </summary>
	Timer m_timer;

	sptr<IOComponent> m_io;
};


