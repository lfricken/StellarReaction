#include "Scoreboard.hpp"

Scoreboard::Scoreboard(const ScoreboardData& rData) : m_gameType(rData.gameType), m_scoreMap(rData.scoreMap)
{

}
Scoreboard::~Scoreboard()
{

}
void Scoreboard::updateScoreboard(ScoreboardData& rData)
{
	m_gameType = rData.gameType;
	m_scoreMap =  rData.scoreMap;
}
GameType Scoreboard::getGameType()
{
	return m_gameType;
}
Map<String, PlayerScore> Scoreboard::getScoreMap()
{
	return m_scoreMap;
}
