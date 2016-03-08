#include "stdafx.h"
#include "gtest/gtest.h"
#include "Chunk.hpp"
#include "Module.hpp"
#include "Reactor.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"
#include <stdio.h>

using namespace std;

TEST(ReactorTest, shipMovesOnDeath)
{
	/*
		1. Create chunkdata
		2. Create reactorData
		3. Add reactorData to chunkData
		4. Create chunk with chunkdata
		5. Create reactor with reactorData
	*/

	//create chunkData
	ChunkData* testChunkData = new ChunkData();
	testChunkData->pParent = &game.getUniverse();

	//create ReactorData
	ReactorData* testReactorData = new ReactorData();
	testChunkData->moduleData.push_back(sptr<ModuleData>(testReactorData));

	//create Chunk
	Chunk* testChunk = new Chunk(*testChunkData);

	//Add chunk to universe
	game.getUniverse().add(testChunk);
//	game.getUniverse().getBlueprints().storeRoster("C:\\Users\\leon\\Desktop\\Projects\\StellarReaction\\blueprints/");



	//Get the location, move the chunk away from origin
	b2Body* chunkBodyPtr= testChunk->getBodyPtr();
	chunkBodyPtr->SetTransform(b2Vec2(20, 20), 0.5);
	b2Vec2 origPos = chunkBodyPtr->GetPosition();

	//Damage the reactor, hopefully killing it
	dynamic_cast<ShipModule*>(testChunk->getModuleList()[0].get())->damage(1000);


	game.runTicks(1);
	b2Vec2 afterDeathPos = testChunk->getBodyPtr()->GetPosition();
	ASSERT_NE(origPos.x, afterDeathPos.x);
	ASSERT_GT(afterDeathPos.x, -8);
	ASSERT_LT(afterDeathPos.x, 8);

}

TEST(ReactorTest, shipHealsOnDeath)
{
	/*
	1. Create chunkdata
	2. Create reactorData
	3. Add reactorData to chunkData
	4. Create chunk with chunkdata
	5. Create reactor with reactorData
	*/

	//create chunkData
	ChunkData* testChunkData = new ChunkData();
	testChunkData->pParent = &game.getUniverse();

	//create ReactorData
	ReactorData* testReactorData = new ReactorData();
	testChunkData->moduleData.push_back(sptr<ModuleData>(testReactorData));

	//create Chunk
	Chunk* testChunk = new Chunk(*testChunkData);

	//Add chunk to universe
	game.getUniverse().add(testChunk);
	//game.getUniverse().getBlueprints().storeRoster("blueprints/");

	//Get the location, move the chunk away from origin
	b2Body* chunkBodyPtr = testChunk->getBodyPtr();
	chunkBodyPtr->SetTransform(b2Vec2(20, 20), 0.5);
	b2Vec2 origPos = chunkBodyPtr->GetPosition();

	//Damage the reactor, hopefully killing it
	dynamic_cast<ShipModule*>(testChunk->getModuleList()[0].get())->damage(1000);

	game.runTicks(1);

	ASSERT_TRUE(dynamic_cast<ShipModule*>(testChunk->getModuleList()[0].get())->isFunctioning());

}

//TEST(ReactorTest, debrisShouldDisappearAfter3Seconds)
//{
//	/*
//	1. Create chunkdata
//	2. Create reactorData
//	3. Add reactorData to chunkData
//	4. Create chunk with chunkdata
//	5. Create reactor with reactorData
//	*/
//
//	//create chunkData
//	ChunkData* testChunkData = new ChunkData();
//	testChunkData->pParent = &game.getUniverse();
//
//	//create ReactorData
//	ReactorData* testReactorData = new ReactorData();
//	testChunkData->moduleData.push_back(sptr<ModuleData>(testReactorData));
//
//	//create Chunk
//	Chunk* testChunk = new Chunk(*testChunkData);
//
//	//Add chunk to universe
//	game.getUniverse().add(testChunk);
//	//game.getUniverse().getBlueprints().storeRoster("blueprints/");
//
//	//Get the location, move the chunk away from origin
//	b2Body* chunkBodyPtr = testChunk->getBodyPtr();
//	chunkBodyPtr->SetTransform(b2Vec2(20, 20), 0.5);
//	b2Vec2 origPos = chunkBodyPtr->GetPosition();
//
//	//Damage the reactor, hopefully killing it
//	dynamic_cast<ShipModule*>(testChunk->getModuleList()[0].get())->damage(1000);
//
//	game.runTicks(1);
//
//	EXPECT_FALSE(game.getUniverse().getDebris().empty());
//
//	game.runTime(3.5);
//
//	ASSERT_TRUE(game.getUniverse().getDebris().empty());
//	
//}