#include "stdafx.h"
#include "gtest/gtest.h"
#include "Stealth.hpp"
#include "Universe.hpp"
#include "Turret.hpp"

extern Game game;

TEST(GameTest, RunTicks)
{
	game.restartTest();

	game.runTicks(5);
	EXPECT_EQ(true, game.getUniverse().isPaused());
}
TEST(GameTest, RunTime)
{
	game.restartTest();

	game.runTime(0.2f);
	EXPECT_EQ(true, game.getUniverse().isPaused());
}