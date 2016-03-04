#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include "Timer.hpp"
#include "Universe.hpp"
#include <map>

struct ScoreboardData;
struct PlayerScore;

enum class GameType
{
	Deathmatch,
	CTF,
};

class Scoreboard
{
public:
	Scoreboard();
	Scoreboard(ScoreboardData& data);
	~Scoreboard();
	void updateScoreboard(ScoreboardData& data);
	void setTimer(float time);
	GameType getGameType();
	std::map<std::string, PlayerScore> getScoreMap();
	Timer getTimer();
protected:
	/*virtual void pack(sf::Packet& rPacket);
	virtual void unpack(sf::Packet& rPacket);*/
private:
	GameType m_gameType;
	std::map<std::string, PlayerScore> m_scoreMap;
	Timer m_gameTimer;
};

struct ScoreboardData
{
	ScoreboardData() : 
		gameType(GameType::Deathmatch),
		scoreMap(),
		gameTimer()
	{
	
	}
	
	virtual ScoreboardData* clone() const
	{
		return new ScoreboardData(*this);
	}
	
	GameType gameType;
	std::map<std::string, PlayerScore> scoreMap;
	Timer gameTimer;
};

struct PlayerScore
{
	PlayerScore() :
		kills(),
		deaths(),
		assists(),
		objectiveCaptures(),
		money()
	{}
	
	int kills;
	int deaths;
	int assists;
	int objectiveCaptures;
	int money;
};

#endif
