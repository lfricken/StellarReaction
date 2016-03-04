#include "Scoreboard.hpp"

Scoreboard::Scoreboard(ScoreboardData& rData) : m_gameType(rData.gameType), m_scoreMap(rData.scoreMap), m_gameTimer(rData.gameTimer)
{
}

Scoreboard::~Scoreboard()
{
}

void Scoreboard::updateScoreboard(ScoreboardData& rData)
{
	m_gameType = rData.gameType;
	m_scoreMap =  rData.scoreMap;
	m_gameTimer = rData.gameTimer;
}

void Scoreboard::setTimer(float time)
{
	m_gameTimer = Timer(time)
}

GameType getGameType()
{
	return m_gameType;
}

std::map<std::string, PlayerScore> getScoreMap()
{
	return m_scoreMap;
}

Timer getTimer()
{
	return m_gameTimer;
}
