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

void teleportParamTest(const vector<b2Vec2>& cases)
{
	for (auto it = cases.begin(); it != cases.end(); ++it)
	{
		b2Vec2 far = *it;

		EXPECT_TRUE(game.getUniverse().isClear(far, 3.0f, NULL));

		//Put Ship There
		ChunkData* c = game.getUniverse().getBlueprints().getChunkSPtr("Anubis")->clone();
		c->bodyComp.coords = far;
		Chunk* testShip = c->generate(&game.getUniverse());
		game.getUniverse().add(testShip);

		//Shouldn't Be Clear
		EXPECT_FALSE(game.getUniverse().isClear(far, 3.0f, NULL));

		//Should Be Clear if it's us
		EXPECT_TRUE(game.getUniverse().isClear(far, 3.0f, testShip));
	}
}

TEST(TeleportTest, IsClear)
{
	game.restartTest();

	vector<b2Vec2> vectors = {
		b2Vec2(200, 200),
		b2Vec2(100, 100)
	};

	teleportParamTest(vectors);
}