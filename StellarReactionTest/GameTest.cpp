#include "stdafx.h"
#include "gtest/gtest.h"
#include "Stealth.hpp"
#include "Universe.hpp"
#include "Turret.hpp"

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
	//game.runTicks(10);
	//EXPECT_EQ(true, game.getUniverse().isPaused());
}
TEST(GameTest, runTime)
{
	//game.runTime(0.3f);
	//EXPECT_EQ(true, game.getUniverse().isPaused());
}