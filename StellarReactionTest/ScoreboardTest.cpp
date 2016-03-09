#include "stdafx.h"
#include "gtest/gtest.h"
#include "Universe.hpp"
#include "Scoreboard.hpp"
#include "Game.hpp"
#include "Stealth.hpp"

extern Game game;


TEST(ScoreboardTest, initalScores)
{
	ChunkData* testChunkData = new ChunkData();
	testChunkData->pParent = &game.getUniverse();

	//create TurretData
	StealthData* testModuleData = new StealthData();
	testChunkData->moduleData.push_back(sptr<ModuleData>(testModuleData));

	//create Chunk
	Chunk* testChunk = new Chunk(*testChunkData);

	//Add chunk to universe
	game.getUniverse().add(testChunk);
	game.runTicks(1);

	int initialScore = testChunk->getScore();
	EXPECT_EQ(0, initialScore);
}

TEST(ScoreboardTest, increaseScore)
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

	//create TurretData
	StealthData* testModuleData = new StealthData();
	testChunkData->moduleData.push_back(sptr<ModuleData>(testModuleData));

	//create Chunk
	Chunk* testChunk = new Chunk(*testChunkData);

	//Add chunk to universe
	game.getUniverse().add(testChunk);
	//game.getUniverse().getBlueprints().storeRoster("blueprints/");
	//game.getUniverse().getBlueprints().storeChunk("Asteroid.bp");

	sf::Packet packet;
	packet << 1000 << testChunk->getModuleList()[0]->getFixtureComponent().getIOPos();
	Message mess;
	mess.reset(testChunk->getModuleList()[0]->getFixtureComponent().getIOPos() , "damage", packet, 0.f, false);
	game.getUniverse().getUniverseIO().recieve(mess);

	game.runTicks(1);

	int score = testChunk->getScore();
	EXPECT_EQ(1, score);
}