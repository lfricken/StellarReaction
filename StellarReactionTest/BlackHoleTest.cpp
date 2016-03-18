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


TEST(BlackHoleTest, blackHoleAppliesForce)
{
	//create black hole
	game.getUniverse().getBlueprints().loadBlueprints("blueprints/");
	ChunkData* bh = game.getUniverse().getBlueprints().getChunkSPtr("DefaultBlackHole")->clone();
	bh->bodyComp.coords = b2Vec2(0, 0);
	Chunk* hole = bh->generate(&game.getUniverse());
	game.getUniverse().add(hole);
	//create chunk
	ChunkData* c = game.getUniverse().getBlueprints().getChunkSPtr("DefaultChunk")->clone();
	c->bodyComp.coords = b2Vec2(-10, 10);
	Chunk* testShip = c->generate(&game.getUniverse());
	game.getUniverse().add(testShip);
	//set chunk to move horizontally
	b2Vec2 orgVel = b2Vec2(10, 0);
	testShip->getBodyPtr()->SetLinearVelocity(orgVel);
	//run time and check that ship now has vertical velocity
	game.runTime(0.5f);
	b2Vec2 newVel = testShip->getBodyPtr()->GetLinearVelocity();
	ASSERT_NE(orgVel.y, newVel.y);
}

TEST(BlackHoleTest, blackHoleDealsDamage)
{
	//create chunk
	ChunkData* c = game.getUniverse().getBlueprints().getChunkSPtr("DefaultChunk")->clone();
	c->bodyComp.coords = b2Vec2(-10, 10);
	Chunk* testShip = c->generate(&game.getUniverse());
	game.getUniverse().add(testShip);
	//set velocity of chunk
	b2Vec2 orgVel = b2Vec2(10, 0);
	testShip->getBodyPtr()->SetLinearVelocity(orgVel);
	//run time and check that a module has taken damage
	game.runTime(0.5f);
	int currentHealth = dynamic_cast<ShipModule*>(testShip->getModuleList()[1].get())->getHealth().getHealth();
	int maxHealth = dynamic_cast<ShipModule*>(testShip->getModuleList()[1].get())->getHealth().getMaxHealth();
	ASSERT_LT(currentHealth, maxHealth);
}