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


TEST(TeleportTest, testIsClear)
{
	//check that it is clear
	b2Vec2 farAway = b2Vec2(200, 200);
	ASSERT_TRUE(game.getUniverse().isClear(farAway, 3.0f, NULL));
	//spawn ship
	ChunkData* c = game.getUniverse().getBlueprints().getChunkSPtr("DefaultChunk")->clone();
	c->bodyComp.coords = farAway;
	Chunk* testShip = c->generate(&game.getUniverse());
	game.getUniverse().add(testShip);
	//should not be clear
	ASSERT_FALSE(game.getUniverse().isClear(farAway, 3.0f, NULL));
	//should be clear if we exclude the testShip
	ASSERT_TRUE(game.getUniverse().isClear(farAway, 3.0f, testShip->getBodyPtr()));
}