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
TEST(GameTest, RestartingManyTimes)
{
	const int numRestarts = 30;
	for(int i = 0; i < numRestarts; ++i)
	{
		game.restartTest();
	}
	for(int i = 0; i < numRestarts; ++i)
	{
		Message host("networkboss", "launch", voidPacket, 0, false);
		game.getCoreIO().recieve(host);
	}
	for(int i = 0; i < numRestarts; ++i)
	{
		game.restartTest();
	}

	EXPECT_EQ(true, true);
}
