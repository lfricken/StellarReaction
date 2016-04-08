#include "stdafx.h"
#include "gtest/gtest.h"
#include "Universe.hpp"
#include "Scoreboard.hpp"
#include "Game.hpp"
#include "Weapon.hpp"
#include "Stealth.hpp"

extern Game game;

TEST(ScoreboardTest, initalScores)
{
	Message host("networkboss", "launch", voidPacket, 0, false);
	game.getCoreIO().recieve(host);

	ChunkData* testChunkData = new ChunkData();

	//create TurretData
	StealthData* testModuleData = new StealthData();
	testChunkData->moduleData.push_back(sptr<ModuleData>(testModuleData));

	//create Chunk
	Chunk* testChunk = testChunkData->generate(&game.getUniverse());

	//Add chunk to universe
	game.getUniverse().add(testChunk);
	game.runTicks(1);

	int initialScore = testChunk->getScore();
	EXPECT_EQ(0, initialScore);
}
TEST(ScoreboardTest, increaseScore)
{
	Message host("networkboss", "launch", voidPacket, 0, false);
	game.getCoreIO().recieve(host);

	//create chunkData
	ChunkData* testChunkData = new ChunkData();

	//create TurretData
	StealthData* testModuleData = new StealthData();
	testChunkData->moduleData.push_back(sptr<ModuleData>(testModuleData));

	//create Chunk
	Chunk* testChunk = testChunkData->generate(&game.getUniverse());

	//Add chunk to universe
	game.getUniverse().add(testChunk);
	//game.getUniverse().getBlueprints().loadBlueprints("blueprints/");
	//game.getUniverse().getBlueprints().storeChunk("Asteroid.bp");

	int pos = testChunk->getModuleList()[0]->getFixtureComponent().getIOPos();
	Weapon::damage(&game.getUniverse().getUniverseIO(), pos, 1000, pos);

	game.runTicks(1);

	int score = testChunk->getScore();
	EXPECT_EQ(1, score);
}


