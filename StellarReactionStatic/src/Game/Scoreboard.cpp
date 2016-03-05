#include "Scoreboard.hpp"

Scoreboard::Scoreboard(const ScoreboardData& rData) : m_gameType(rData.gameType), m_scoreMap(rData.scoreMap), m_gameTimer(rData.gameTimer)
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
	m_gameTimer = Timer(time);
}

GameType Scoreboard::getGameType()
{
	return m_gameType;
}

std::map<std::string, PlayerScore> Scoreboard::getScoreMap()
{
	return m_scoreMap;
}

Timer Scoreboard::getTimer()
{
	return m_gameTimer;
}
