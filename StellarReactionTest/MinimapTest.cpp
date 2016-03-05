#include "stdafx.h"
#include "gtest/gtest.h"
#include "Game.hpp"
#include "Minimap.hpp"
#include "Universe.hpp"
#include "NetworkBoss.hpp"
#include "Player.hpp"

extern Game game;

class MiniMapTest : public ::testing::Test
{
protected:
	virtual void setUp()
	{

	}

	virtual void tearDown()
	{

	}
};

TEST(MiniMapTest, minimap)
{
	Message host("networkboss", "launch", voidPacket, 0, false);
	game.getCoreIO().recieve(host);


	game.runTicks(5);


	//game.getUniverse().getgoList().size();
	//game.getLocalPlayer().radarsize();

	EXPECT_EQ(game.getLocalPlayer().radarsize(), game.getUniverse().getgoList().size());
}
TEST(MiniMapTest, minimap2)
{
	game.runTime(0.3f);
	EXPECT_EQ(true, game.getUniverse().isPaused());
}