#include "stdafx.h"
#include "gtest/gtest.h"
#include "Chunk.hpp"
#include "Module.hpp"
#include "Universe.hpp"
#include "ShipModule.hpp"
#include "BlueprintLoader.hpp"
#include <stdio.h>

extern Game game;

vector<Chunk*> initFindNearestTest()
{
	vector<Chunk*> chunkList;
	//index 0 is null
	chunkList.push_back(NULL);
	//index 1 is team 1
	ChunkData* c = game.getUniverse().getBlueprints().getChunkSPtr("Anubis")->clone();
	c->bodyComp.coords = b2Vec2(20, 20);
	Chunk* testShip = c->generate(&game.getUniverse());
	testShip->getBodyComponent().setTeam(1);
	game.getUniverse().add(testShip);
	chunkList.push_back(testShip);
	//index 2 is team 2
	c->bodyComp.coords = b2Vec2(-20, -20);
	testShip = c->generate(&game.getUniverse());
	testShip->getBodyComponent().setTeam(2);
	game.getUniverse().add(testShip);
	chunkList.push_back(testShip);
	//index 3 is asteroid
	ChunkData* a = game.getUniverse().getBlueprints().getChunkSPtr("Asteroid")->clone();
	a->bodyComp.coords = b2Vec2(20, -20);
	testShip = a->generate(&game.getUniverse());
	game.getUniverse().add(testShip);
	chunkList.push_back(testShip);

	return chunkList;
}

void findNearestParamTest(const vector<b2Vec3>& cases, const vector<Chunk*>& chunks)
{
	for (auto it = cases.begin(); it != cases.end(); ++it)
	{
		b2Vec3 testCase = *it;
		b2Vec2 pos = b2Vec2(testCase.x, testCase.y);
		int expectedIndex = testCase.z;

		Chunk* actualNearest = game.getUniverse().getNearestChunkExcept(pos, NULL);
		ASSERT_EQ(actualNearest, chunks.at(expectedIndex));
	}
}

TEST(FindNearestTest, findShips)
{
	game.restartTest();
	vector<Chunk*> chunks = initFindNearestTest();

	//(x_pos, y_pos, expected index)
	vector<b2Vec3> vectors = {
		b2Vec3(1, 1, 1),
		b2Vec3(-1, -1, 2),
		b2Vec3(200, -200, 3)
	};

	findNearestParamTest(vectors, chunks);
}