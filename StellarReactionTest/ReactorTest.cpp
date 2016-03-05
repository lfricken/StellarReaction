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
	sptr<ChunkData> cd(new ChunkData());
	sptr<ModuleData> rd(new ReactorData());
	ASSERT_TRUE(cd);
	ASSERT_TRUE(rd);
	cd->moduleData.push_back(rd);


	Chunk* c = new Chunk(*cd);
	int x = 5;
	//rd->chunkParent = c;

	//Reactor* r = new Reactor(*dynamic_cast<ReactorData*>(&*rd));
	

	Universe* uni = &game.getUniverse();
	BlueprintLoader* bpl = &uni->getBlueprints();
	bpl->storeChunk("TestReactor.bp");
	bpl->storeChunk("TestChunk.bp");*/
}