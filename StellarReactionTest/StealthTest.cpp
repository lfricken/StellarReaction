#include "stdafx.h"
#include "gtest/gtest.h"
#include "Stealth.hpp"
#include "Universe.hpp"

extern Game game;

TEST(StealthTest, StealthInactive)
{
	game.restartTest();

	//Create Chunk
	ChunkData testChunkData;

	//Create Stealth
	StealthData data;
	data.energyConsumption = 0;
	testChunkData.moduleData.push_back(sptr<ModuleData>(new StealthData(data)));

	//Add Chunk
	Chunk* testChunk = testChunkData.generate(&game.getUniverse());
	game.getUniverse().add(testChunk);

	game.runTicks(3);

	//verify ship is not in stealth
	EXPECT_FALSE(testChunk->isStealth());
}

TEST(StealthTest, StealthActive)
{
	game.restartTest();

	//Create Chunk
	ChunkData testChunkData;

	//Create Stealth
	StealthData data;
	data.energyConsumption = 0;
	testChunkData.moduleData.push_back(sptr<ModuleData>(new StealthData(data)));

	//Add Chunk
	Chunk* testChunk = testChunkData.generate(&game.getUniverse());
	game.getUniverse().add(testChunk);

	//Activate Stealth Module
	CommandInfo com;
	com.directives[Directive::Stealth] = true;
	testChunk->getModuleList()[0]->directive(com);

	game.runTicks(3);

	//verify ship is in stealth mode
	EXPECT_TRUE(testChunk->isStealth());
}
