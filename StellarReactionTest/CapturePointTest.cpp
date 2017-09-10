#include "stdafx.h"
#include "gtest/gtest.h"
#include "Chunk.hpp"
#include "Module.hpp"
#include "CaptureArea.hpp"
#include "CaptureGraphic.hpp"
#include "Universe.hpp"
#include "ShipModule.hpp"
#include "BlueprintLoader.hpp"
#include <stdio.h>

extern Game game;

Chunk* capturePoint;
Chunk* testShip1;
Chunk* testShip2;

TEST(CapturePointTest, singlePlayerInPoint)
{
	//create capture point
	ChunkData* sData = game.getUniverse().getBlueprints().getChunkSPtr("CapturePointCCW")->clone();
	sData->bodyComp.coords = b2Vec2(50, 50);
	capturePoint = sData->generate(&game.getUniverse());
	game.getUniverse().add(capturePoint);

	//create chunk on team 1
	ChunkData* c1 = game.getUniverse().getBlueprints().getChunkSPtr("TestChunk")->clone();
	c1->bodyComp.coords = b2Vec2(51, 51);
	testShip1 = c1->generate(&game.getUniverse());
	testShip1->getBodyComponent().setTeam(Team::One);
	game.getUniverse().add(testShip1);

	//run time, check current owner of capturepoint is team 1
	game.runTime(0.1f);
	int curTeam = (int)dynamic_cast<CaptureArea*>(&*capturePoint->getModuleList()[0])->getCurrentTeam();
	ASSERT_EQ(1, curTeam);
}
TEST(CapturePointTest, twoTeamsInPoint)
{
	//create chunk on team 2
	ChunkData* c1 = game.getUniverse().getBlueprints().getChunkSPtr("TestChunk")->clone();
	c1->bodyComp.coords = b2Vec2(49, 49);
	testShip2 = c1->generate(&game.getUniverse());
	testShip2->getBodyComponent().setTeam(Team::Two);
	game.getUniverse().add(testShip2);

	//run time, current owner is still team 1, but capture point is conflicted
	game.runTime(0.1f);
	int curTeam = (int)dynamic_cast<CaptureArea*>(&*capturePoint->getModuleList()[0])->getCurrentTeam();
	bool conflicted = dynamic_cast<CaptureArea*>(&*capturePoint->getModuleList()[0])->isConflicted();
	ASSERT_EQ(1, curTeam);
	ASSERT_TRUE(conflicted);
}
