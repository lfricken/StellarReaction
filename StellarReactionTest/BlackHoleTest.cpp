#include "stdafx.h"
#include "gtest/gtest.h"
#include "Chunk.hpp"
#include "Module.hpp"
#include "Reactor.hpp"
#include "Universe.hpp"
#include "ShipModule.hpp"
#include "BlueprintLoader.hpp"
#include <stdio.h>

using namespace std;

extern Game game;

TEST(BlackHoleTest, AppliesForce)
{
	game.restartTest();

	//Black Hole
	ChunkData* bh = game.getUniverse().getBlueprints().getChunkSPtr("BlackHole")->clone();
	bh->bodyComp.coords = b2Vec2(0, 0);
	Chunk* hole = bh->generate(&game.getUniverse());
	game.getUniverse().add(hole);

	//Ship
	ChunkData* c = game.getUniverse().getBlueprints().getChunkSPtr("Anubis")->clone();
	c->bodyComp.coords = b2Vec2(2, 2);

	Chunk* testShip = c->generate(&game.getUniverse());
	game.getUniverse().add(testShip);

	//Moved
	game.runTicks(2);
	b2Vec2 newVel = testShip->getBodyPtr()->GetLinearVelocity();
	EXPECT_NE(newVel.x, 0);
	EXPECT_NE(newVel.y, 0);
}
TEST(BlackHoleTest, DealsDamage)
{
	game.restartTest();

	//Black Hole
	ChunkData* blackholeData = game.getUniverse().getBlueprints().getChunkSPtr("BlackHole")->clone();
	blackholeData->bodyComp.coords = b2Vec2(100, 0);
	Chunk* hole = blackholeData->generate(&game.getUniverse());
	game.getUniverse().add(hole);

	//Create Ship
	ChunkData* shipData = game.getUniverse().getBlueprints().getChunkSPtr("Anubis")->clone();
	shipData->bodyComp.coords = b2Vec2(100, 0);
	Chunk* testShip = shipData->generate(&game.getUniverse());
	game.getUniverse().add(testShip);

	//Taken Damage
	game.runTime(0.4f);
	int currentHealth = dynamic_cast<ShipModule*>(testShip->getModuleList()[0].get())->getHealth().getHealth();
	int maxHealth = dynamic_cast<ShipModule*>(testShip->getModuleList()[0].get())->getHealth().getMaxHealth();
	b2Vec2 pos = testShip->getBodyComponent().getPosition();
	EXPECT_LT(currentHealth, maxHealth);// || pos.x != 100

}

