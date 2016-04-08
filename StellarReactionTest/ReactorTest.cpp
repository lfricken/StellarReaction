#include "stdafx.h"
#include "gtest/gtest.h"
#include "Chunk.hpp"
#include "Module.hpp"
#include "Reactor.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"
#include "Weapon.hpp"
#include <stdio.h>

using namespace std;

extern Game game;

TEST(ReactorTest, ShipMovesOnDeath)
{
	game.restartTest();

	//Create Chunk
	ChunkData* testChunkData = new ChunkData();

	//Create Reactor
	ReactorData* testReactorData = new ReactorData();
	testChunkData->moduleData.push_back(sptr<ModuleData>(testReactorData));

	//Add Chunk
	Chunk* testChunk = testChunkData->generate(&game.getUniverse());
	game.getUniverse().add(testChunk);

	//Get Location, Move Chunk
	b2Body* chunkBodyPtr= testChunk->getBodyPtr();
	chunkBodyPtr->SetTransform(b2Vec2(200, 200), 0.5);
	b2Vec2 origPos = chunkBodyPtr->GetPosition();

	//Kill Reactor Module
	int pos = testChunk->getModuleList()[0]->getFixtureComponent().getIOPos();
	Weapon::damage(&game.getUniverse().getUniverseIO(), pos, 1000, pos);

	//Run Test
	game.runTicks(2);
	b2Vec2 afterDeathPos = testChunk->getBodyPtr()->GetPosition();
	EXPECT_NE(origPos.x, afterDeathPos.x);
	EXPECT_GT(afterDeathPos.x, -9);
	EXPECT_LT(afterDeathPos.x, 9);
}
TEST(ReactorTest, ShipHealsOnDeath)
{
	game.restartTest();

	//Create Chunk
	ChunkData* testChunkData = new ChunkData();

	//Create Reactor
	ReactorData* testReactorData = new ReactorData();
	testChunkData->moduleData.push_back(sptr<ModuleData>(testReactorData));

	//Add Chunk
	Chunk* testChunk = testChunkData->generate(&game.getUniverse());
	game.getUniverse().add(testChunk);

	//Get Location, Move Chunk
	b2Body* chunkBodyPtr = testChunk->getBodyPtr();
	chunkBodyPtr->SetTransform(b2Vec2(200, 200), 0.5);
	b2Vec2 origPos = chunkBodyPtr->GetPosition();

	//Kill Reactor Module
	int pos = testChunk->getModuleList()[0]->getFixtureComponent().getIOPos();
	Weapon::damage(&game.getUniverse().getUniverseIO(), pos, 1000, pos);

	//Run Test
	game.runTicks(2);
	EXPECT_TRUE(dynamic_cast<ShipModule*>(testChunk->getModuleList()[0].get())->isFunctioning());
}
TEST(ReactorTest, NoDebris)
{
	//Deleted Test.
	//Debris system causes lag.
	//Debris system either needs to be graphics only or have preallocated debris chunks.
}

