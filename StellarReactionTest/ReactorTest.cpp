#include "stdafx.h"
#include "gtest/gtest.h"
#include "Chunk.hpp"
#include "Module.hpp"
#include "Reactor.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"
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
	sf::Packet packet;
	packet << 1000 << testChunk->getModuleList()[0]->getFixtureComponent().getIOPos();
	Message mess;
	mess.reset(testChunk->getModuleList()[0]->getFixtureComponent().getIOPos(), "damage", packet, 0.f, false);
	game.getUniverse().getUniverseIO().recieve(mess);


	game.runTicks(30);
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
	//game.getUniverse().getBlueprints().loadBlueprints("blueprints/");

	//Get the location, move the chunk away from origin
	b2Body* chunkBodyPtr = testChunk->getBodyPtr();
	chunkBodyPtr->SetTransform(b2Vec2(20, 20), 0.5);
	b2Vec2 origPos = chunkBodyPtr->GetPosition();

	//destroy the reactor
	sf::Packet packet;
	packet << 1000 << testChunk->getModuleList()[0]->getFixtureComponent().getIOPos();
	Message mess;
	mess.reset(testChunk->getModuleList()[0]->getFixtureComponent().getIOPos(), "damage", packet, 0.f, false);
	game.getUniverse().getUniverseIO().recieve(mess);

	game.runTicks(30);

	ASSERT_TRUE(dynamic_cast<ShipModule*>(testChunk->getModuleList()[0].get())->isFunctioning());

}

TEST(ReactorTest, debrisShouldDisappearAfter3Seconds)
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
	//game.getUniverse().getBlueprints().loadBlueprints("blueprints/");

	//Get the location, move the chunk away from origin
	b2Body* chunkBodyPtr = testChunk->getBodyPtr();
	chunkBodyPtr->SetTransform(b2Vec2(20, 20), 0.5);
	b2Vec2 origPos = chunkBodyPtr->GetPosition();

	//destroy the reactor
	sf::Packet packet;
	packet << 1000 << testChunk->getModuleList()[0]->getFixtureComponent().getIOPos();
	Message mess;
	mess.reset(testChunk->getModuleList()[0]->getFixtureComponent().getIOPos(), "damage", packet, 0.f, false);
	game.getUniverse().getUniverseIO().recieve(mess);

	game.runTicks(30);

	EXPECT_FALSE(game.getUniverse().getDebris().empty());

	game.runTime(3.5);

	ASSERT_TRUE(game.getUniverse().getDebris().empty());
	
}