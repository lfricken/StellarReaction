#include "stdafc.h"
#include "gtest/gtest.h"
#include "Universe.hpp"
#include "Scoreboard.hpp"

TEST(ScoreboardTest, testGameType)
{
        game.runTime(10.0f);
        Universe uni = game.getUniverse();
        Scoreboard score = uni.getScoreboard();
        EXPECT_EQ(GameType::Deathmatch, score.getGameType());
}

TEST(ScoreboardTest, testScoreMap)
{
	game.runTime(10.0f);
	Universe uni = game.getUniverse();
	Scoreboard score = uni.getScoreboard();
	std::map<std::string, PlayerScore> emptyMap = std::map<std::string, PlayerScore>();
	EXPECT_EQ(emptyMap , score.getScoreMap());
} 
