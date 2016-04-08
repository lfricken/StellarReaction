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


TEST(ReactorTest, shipMovesOnDeath)
{
	/*
		1. Create chunkdata
		2. Create reactorData
		3. Add reactorData to chunkData
		4. Create chunk with chunkdata
		5. Create reactor with reactorData
	*/

	//create chunkData, respawns at (100,100)
	ChunkData* testChunkData = new ChunkData();
	testChunkData->pParent = &game.getUniverse();
	testChunkData->bodyComp.coords = b2Vec2(100, 100);

	//create ReactorData
	ReactorData* testReactorData = new ReactorData();
	testReactorData->respawnTime = 0.05f;
	testReactorData->waitTime = 0.05f;
	testChunkData->moduleData.push_back(sptr<ModuleData>(testReactorData));

	//create Chunk
	Chunk* testChunk = new Chunk(*testChunkData);
	//Add chunk to universe
	game.getUniverse().add(testChunk);
	game.runTicks(2);

	//Get the location, move the chunk away from original position
	b2Body* chunkBodyPtr= testChunk->getBodyPtr();
	chunkBodyPtr->SetTransform(b2Vec2(20, 20), 0.5);
	b2Vec2 origPos = chunkBodyPtr->GetPosition();
	game.runTicks(5);

	//destroy the reactor
	int pos = testChunk->getModuleList()[0]->getFixtureComponent().getIOPos();
	Weapon::damage(&game.getUniverse().getUniverseIO(), pos, 2000, pos);


	game.runTime(0.3f);
	b2Vec2 afterDeathPos = testChunk->getBodyPtr()->GetPosition();
	ASSERT_NE(origPos.x, afterDeathPos.x);
	ASSERT_GT(afterDeathPos.x, 97);
	ASSERT_LT(afterDeathPos.x, 102);
	ASSERT_GT(afterDeathPos.y, 97);
	ASSERT_LT(afterDeathPos.y, 102);

}

TEST(ReactorTest, shipHealsOnDeath)
{
	//create chunkData, respawns at (100,100)
	ChunkData* testChunkData = new ChunkData();
	testChunkData->pParent = &game.getUniverse();
	testChunkData->bodyComp.coords = b2Vec2(120, 120);

	//create ReactorData
	ReactorData* testReactorData = new ReactorData();
	testReactorData->respawnTime = 0.05f;
	testReactorData->waitTime = 0.05f;
	testChunkData->moduleData.push_back(sptr<ModuleData>(testReactorData));

	//create Chunk
	Chunk* testChunk = new Chunk(*testChunkData);
	//Add chunk to universe
	game.getUniverse().add(testChunk);
	game.runTicks(2);

	//Get the location, move the chunk away from original position
	b2Body* chunkBodyPtr = testChunk->getBodyPtr();
	chunkBodyPtr->SetTransform(b2Vec2(20, 20), 0.5);
	b2Vec2 origPos = chunkBodyPtr->GetPosition();
	game.runTicks(5);

	//destroy the reactor
	int pos = testChunk->getModuleList()[0]->getFixtureComponent().getIOPos();
	Weapon::damage(&game.getUniverse().getUniverseIO(), pos, 2000, pos);

	game.runTime(0.3f);
	
	ASSERT_TRUE(dynamic_cast<ShipModule*>(testChunk->getModuleList()[0].get())->isFunctioning());
}