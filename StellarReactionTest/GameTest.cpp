#include "stdafx.h"
#include "gtest/gtest.h"
#include "Stealth.hpp"
#include "Universe.hpp"

Game game;

class GameTest : public ::testing::Test
{
protected:
	virtual void setUp()
	{

	}

	virtual void tearDown()
	{

	}
};

TEST(GameTest, runTicks)
{
	game.runTicks(10);
	EXPECT_EQ(true, game.getUniverse().isPaused());
}
TEST(GameTest, runTime)
{
	game.runTime(0.3f);
	EXPECT_EQ(true, game.getUniverse().isPaused());
}

TEST(GameTest, runLoop)
{
	game.runTime(30.0f);
	Universe uni = game.getUniverse();
	std::vector<sptr<GameObject>> objectList = uni.getgoList();
	for (auto it = objectList.begin(); it != objectList.end(); ++it)
	{
		int score = (*it)->getScore();
		EXPECT_EQ(0, score);
	}
}




