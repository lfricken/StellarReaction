#include "stdafx.h"
#include "gtest/gtest.h"
#include "Chunk.hpp"
#include "Module.hpp"
#include "Turret.hpp"
#include "GrappleWeapon.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"
#include "Controller.hpp"
#include <stdio.h>

using namespace std;

extern Game game;

TEST(GrappleTest, GrappleShip)
{

	/*
	1. Create chunkdata
	2. Create LaserData
	3. Add LaserData to chunkData
	4. Create chunk with chunkdata
	5. Create Grapple with GrappleData
	

	//create chunkData
	ChunkData* testChunkData = new ChunkData();
	testChunkData->pParent = &game.getUniverse();
	testChunkData->bodyComp.coords = b2Vec2(700.0, 700.0);
	//create turretdata
	TurretData* testTurretData = new TurretData();
	GrappleWeaponData* testGrappleWeaponData = new GrappleWeaponData();
	testGrappleWeaponData->ener = 0;
	testGrappleWeaponData->ball = 0;
	testGrappleWeaponData->mis = 0;

	testTurretData->startWep.reset(testGrappleWeaponData);

	testChunkData->moduleData.push_back(sptr<ModuleData>(testTurretData));

	//create GrappleWeaponData
	///testChunkData->moduleData.push_back(sptr<ModuleData>(testGrappleWeaponData));

	//create Chunk
	Chunk* testChunk = new Chunk(*testChunkData);

	//create chunk for the ship 
	//b2Body* chunkBodyPtr = testChunk->getBodyPtr();

	testChunkData->bodyComp.coords.x += 4;
	Chunk* testChunk2 = new Chunk(*testChunkData);



	//Add chunk to universe
	game.getUniverse().add(testChunk);
	game.getUniverse().add(testChunk2);
	//	game.getUniverse().getBlueprints().loadBlueprints( BAD BAD BAD"C:\\Users\\leon\\Desktop\\Projects\\StellarReaction\\blueprints/");

	b2Body* chunkBodyPtr3 = testChunk->getBodyPtr();
	b2Body* chunkBodyPtr4 = testChunk2->getBodyPtr();
	b2Vec2 origPos = chunkBodyPtr3->GetPosition();
	b2Vec2 origPos2 = chunkBodyPtr4->GetPosition();

	testChunk->setAim(b2Vec2(704, 700));

	std::map<Directive, bool> dirs;
	dirs[Directive::FirePrimary] = true;
	testChunk->directive(dirs, true);
	game.runTicks(5);
	dirs[Directive::FirePrimary] = true;
	game.runTicks(30);

    b2Body* chunkBodyPtr = testChunk->getBodyPtr();
	b2Body* chunkBodyPtr2 = testChunk2->getBodyPtr();
	b2Vec2 Pos = chunkBodyPtr->GetPosition();
	b2Vec2 Pos2 = chunkBodyPtr2->GetPosition();

	EXPECT_FALSE(Pos.x == origPos.x);// && Pos.y == origPos.y);



	////destroy the reactor // use Weapon::damage
	//int pos = testChunk->getModuleList()[0]->getFixtureComponent().getIOPos();
	//Weapon::damage(&game.getUniverse().getUniverseIO(), pos, 1000, pos);


	//game.runTicks(30);
	//b2Vec2 afterDeathPos = testChunk->getBodyPtr()->GetPosition();
	//ASSERT_NE(origPos.x, afterDeathPos.x);
	//ASSERT_GT(afterDeathPos.x, -8);
	//ASSERT_LT(afterDeathPos.x, 8);
*/
}
