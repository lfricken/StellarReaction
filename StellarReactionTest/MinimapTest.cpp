#include "stdafx.h"
#include "gtest/gtest.h"
#include "Game.hpp"
#include "Minimap.hpp"
#include "Universe.hpp"
#include "NetworkBoss.hpp"
#include "Player.hpp"

extern Game game;

TEST(MiniMapTest, NumberOfMarkers)
{
	game.restartTest("Alpha Centauri");

	game.runTicks(3);

	int totalItems = game.getUniverse().getgoList().size();
	int radarItems = game.getLocalPlayer().radarsize();

	EXPECT_LE(totalItems, radarItems);
}
