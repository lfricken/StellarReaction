#pragma once

#include "Timer.hpp"
#include "stdafx.hpp"

struct ScoreboardData;
struct PlayerScore;

/// Which game  mode we are in.
enum class GameType
{
	Deathmatch,
	Conquest,
	CTF,
};

/// Used to show the scores of all players.
class Scoreboard
{
public:
	Scoreboard(const ScoreboardData& data);
	~Scoreboard();
	/// Update the scoreboard with new info.
	void updateScoreboard(ScoreboardData& data);
	/// Determine which game mode we are in.
	GameType getGameType();
	/// Get the list of scores, and names of the players.
	Map<String, PlayerScore> getScoreMap();

protected:
	/*virtual void pack(sf::Packet& rPacket);
	virtual void unpack(sf::Packet& rPacket);*/
private:
	GameType m_gameType;
	Map<String, PlayerScore> m_scoreMap;
};

/// All the information the scoreboard needs to display accurately.
struct ScoreboardData
{
	ScoreboardData() :
		gameType(GameType::Deathmatch),
		scoreMap()
	{

	}

	GameType gameType;
	Map<String, PlayerScore> scoreMap;
};

/// Score information for an individual player.
struct PlayerScore
{
	PlayerScore() :
		kills(-1),
		deaths(-1),
		assists(-1),
		objectiveCaptures(-1),
		money(-1)
	{
	}

	int kills;
	int deaths;
	int assists;
	int objectiveCaptures;
	int money;
};

