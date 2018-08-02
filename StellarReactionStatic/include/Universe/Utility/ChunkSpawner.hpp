#pragma once

#include "stdafx.hpp"
#include "Timer.hpp"
#include "IOComponent.hpp"
#include "Team.hpp"

class Universe;
struct ChunkDataMessage;

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
	/// <summary>
	/// Picks a random point in a circle.
	/// </summary>
	Vec2 pickRandPoint();

	Universe* m_pUniverse;

	int m_spawnPer; // How many chunks to spawn per spawn event.
	/// <summary>
	/// Should we be trying to spawn things now.
	/// </summary>
	bool m_isEnabled;
	/// <summary>
	/// Blueprints to randomly choose from.
	/// </summary>
	List<String> m_blueprints;
	/// <summary>
	/// Spawn limit. Can only spawn this many per getGame()-> -1 is unlimited.
	/// </summary>
	int m_spawnAmount;
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
	/// <summary>
	/// Type of thing to create, loaded from json.
	/// </summary>
	sptr<ChunkDataMessage> m_chunkData;

	sptr<IOComponent> m_io;
};


