#include "stdafx.h"
#include "gtest/gtest.h"
#include "Stealth.hpp"

Game game;

class StealthTest : public ::testing::Test
{
protected:
	virtual void setUp()
	{

	}

	virtual void tearDown()
	{

	}
};

TEST(StealthTest, stealthActive)
{
	game.run();//TODO write a version of game run that takes a time parameter so we can run it for a certain amount of time, then pause
	EXPECT_EQ(1.0, 1.0);
}
