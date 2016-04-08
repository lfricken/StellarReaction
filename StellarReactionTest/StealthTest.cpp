#include "stdafx.h"
#include "gtest/gtest.h"
#include "Stealth.hpp"
#include "Universe.hpp"

extern Game game;

TEST(StealthTest, StealthActive)
{
	game.restartTest();

	//Create Chunk
	ChunkData testChunkData;
	testChunkData.ballisticData.Max = 22;

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

	EXPECT_TRUE(((ShipModule*)testChunk->getModuleList()[0].get())->isFunctioning());
}
