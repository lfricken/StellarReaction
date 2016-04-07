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

	//create chunkData
	ChunkData* testChunkData = new ChunkData();
	testChunkData->pParent = &game.getUniverse();

	//create ReactorData
	ReactorData* testReactorData = new ReactorData();
	testReactorData->respawnTime = 0.f;
	testChunkData->moduleData.push_back(sptr<ModuleData>(testReactorData));

	//create Chunk
	Chunk* testChunk = new Chunk(*testChunkData);

	//Add chunk to universe
	game.getUniverse().add(testChunk);
//	game.getUniverse().getBlueprints().loadBlueprints( BAD BAD BAD"C:\\Users\\leon\\Desktop\\Projects\\StellarReaction\\blueprints/");



	//Get the location, move the chunk away from origin
	b2Body* chunkBodyPtr= testChunk->getBodyPtr();
	chunkBodyPtr->SetTransform(b2Vec2(20, 20), 0.5);
	b2Vec2 origPos = chunkBodyPtr->GetPosition();

	//destroy the reactor
	int pos = testChunk->getModuleList()[0]->getFixtureComponent().getIOPos();
	Weapon::damage(&game.getUniverse().getUniverseIO(), pos, 1000, pos);


	game.runTicks(60);
	b2Vec2 afterDeathPos = testChunk->getBodyPtr()->GetPosition();
	ASSERT_NE(origPos.x, afterDeathPos.x);
	ASSERT_GT(afterDeathPos.x, -9);
	ASSERT_LT(afterDeathPos.x, 9);

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
	testReactorData->respawnTime = 0.f;
	testChunkData->moduleData.push_back(sptr<ModuleData>(testReactorData));

	//create Chunk
	Chunk* testChunk = new Chunk(*testChunkData);

	//Add chunk to universe
	game.getUniverse().add(testChunk);
	//game.getUniverse().getBlueprints().loadBlueprints("blueprints/");

	//Get the location, move the chunk away from origin
	b2Body* chunkBodyPtr = testChunk->getBodyPtr();
	chunkBodyPtr->SetTransform(b2Vec2(20, 20), 0.5);
	b2Vec2 origPos = chunkBodyPtr->GetPosition();

	//destroy the reactor
	Weapon::damage(&game.getUniverse().getUniverseIO(), testChunk->getModuleList()[0]->getFixtureComponent().getIOPos(), 1000, testChunk->getModuleList()[0]->getFixtureComponent().getIOPos());

	game.runTicks(10);

	ASSERT_TRUE(dynamic_cast<ShipModule*>(testChunk->getModuleList()[0].get())->isFunctioning());

}