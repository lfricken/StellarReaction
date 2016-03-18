#include "stdafx.h"
#include "gtest/gtest.h"
#include "Chunk.hpp"
#include "Module.hpp"
#include "GrappleWeapon.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"
#include <stdio.h>

using namespace std;

extern Game game;




TEST(GrappleTest, shipMovesTowardsTarget)
{
	/*
	1. Create chunkdata
	2. Create LaserData
	3. Add LaserData to chunkData
	4. Create chunk with chunkdata
	5. Create Grapple with GrappleData
	*/

	//create chunkData
	ChunkData* testChunkData = new ChunkData();
	testChunkData->pParent = &game.getUniverse();

	//create LaserData
	LaserWeaponData* testLaserWeaponData = new LaserWeaponData();
	testChunkData->moduleData.push_back(sptr<ModuleData>(testLaserWeaponData));

	//create Chunk
	Chunk* testChunk = new Chunk(*testChunkData);

	//create chunk for the ship 
	b2Body* chunkBodyPtr = testChunk->getBodyPtr();


	//Add chunk to universe
	game.getUniverse().add(testChunk);
	//	game.getUniverse().getBlueprints().loadBlueprints( BAD BAD BAD"C:\\Users\\leon\\Desktop\\Projects\\StellarReaction\\blueprints/");



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
	b2Vec2 afterDeathPos = testChunk->getBodyPtr()->GetPosition();
	ASSERT_NE(origPos.x, afterDeathPos.x);
	ASSERT_GT(afterDeathPos.x, -8);
	ASSERT_LT(afterDeathPos.x, 8);

}
