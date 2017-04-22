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
	testShip1->getBodyComponent().setTeam(1);
	game.getUniverse().add(testShip1);

	//run time, check current owner of capturepoint is team 1
	game.runTime(0.1f);
	int curTeam = dynamic_cast<CaptureArea*>(&*capturePoint->getModuleList()[0])->getCurrentTeam();
	ASSERT_EQ(1, curTeam);
}
TEST(CapturePointTest, twoTeamsInPoint)
{
	//create chunk on team 2
	ChunkData* c1 = game.getUniverse().getBlueprints().getChunkSPtr("TestChunk")->clone();
	c1->bodyComp.coords = b2Vec2(49, 49);
	testShip2 = c1->generate(&game.getUniverse());
	testShip2->getBodyComponent().setTeam(2);
	game.getUniverse().add(testShip2);

	//run time, current owner is still team 1, but capture point is conflicted
	game.runTime(0.1f);
	int curTeam = dynamic_cast<CaptureArea*>(&*capturePoint->getModuleList()[0])->getCurrentTeam();
	bool conflicted = dynamic_cast<CaptureArea*>(&*capturePoint->getModuleList()[0])->isConflicted();
	ASSERT_EQ(1, curTeam);
	ASSERT_TRUE(conflicted);
}
TEST(CapturePointTest, oppositeTeamInPoint)
{
	//move testship1
	testShip1->getBodyPtr()->SetTransform(b2Vec2(70, 70), 0);
	testShip1->getBodyPtr()->ApplyForceToCenter(b2Vec2(5, 5), true);
	//run ticks and ensure team 1 still owns, and not conflicted
	game.runTicks(2);
	int curTeam = dynamic_cast<CaptureArea*>(&*capturePoint->getModuleList()[0])->getCurrentTeam();
	bool conflicted = dynamic_cast<CaptureArea*>(&*capturePoint->getModuleList()[0])->isConflicted();
	ASSERT_EQ(1, curTeam);
	ASSERT_FALSE(conflicted);
	//run time and ensure team 2 now owns point
	game.runTime(0.5f);
	curTeam = dynamic_cast<CaptureArea*>(&*capturePoint->getModuleList()[0])->getCurrentTeam();
	ASSERT_EQ(2, curTeam);
}
TEST(CapturePointTest, asteroidInPoint)
{
	//create asteroid
	ChunkData* c1 = game.getUniverse().getBlueprints().getChunkSPtr("Asteroid")->clone();
	c1->bodyComp.coords = b2Vec2(51, 49);
	Chunk* asteroid = c1->generate(&game.getUniverse());
	game.getUniverse().add(asteroid);
	//run time and ensure team 2 still owns and point is not in conflict
	game.runTicks(2);
	int curTeam = dynamic_cast<CaptureArea*>(&*capturePoint->getModuleList()[0])->getCurrentTeam();
	bool conflicted = dynamic_cast<CaptureArea*>(&*capturePoint->getModuleList()[0])->isConflicted();
	ASSERT_EQ(curTeam, 2);
	ASSERT_FALSE(conflicted);
}