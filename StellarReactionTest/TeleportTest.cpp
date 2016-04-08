#include "stdafx.h"
#include "gtest/gtest.h"
#include "Chunk.hpp"
#include "Module.hpp"
#include "Universe.hpp"
#include "ShipModule.hpp"
#include "BlueprintLoader.hpp"
#include <stdio.h>

using namespace std;

extern Game game;


TEST(TeleportTest, IsClear)
{
	game.restartTest();

	//Select Clear Position
	b2Vec2 farAway = b2Vec2(200, 200);
	EXPECT_TRUE(game.getUniverse().isClear(farAway, 3.0f, NULL));

	//Put Ship There
	ChunkData* c = game.getUniverse().getBlueprints().getChunkSPtr("Anubis")->clone();
	c->bodyComp.coords = farAway;
	Chunk* testShip = c->generate(&game.getUniverse());
	game.getUniverse().add(testShip);

	//Shouldn't Be Clear
	EXPECT_FALSE(game.getUniverse().isClear(farAway, 3.0f, NULL));

	//Should Be Clear if it's us
	EXPECT_TRUE(game.getUniverse().isClear(farAway, 3.0f, testShip->getBodyPtr()));
}