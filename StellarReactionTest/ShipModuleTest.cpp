#include "stdafx.h"
#include "gtest/gtest.h"
#include "Chunk.hpp"
#include "Module.hpp"
#include "Reactor.hpp"
#include "Universe.hpp"
#include "ShipModule.hpp"
#include "BlueprintLoader.hpp"
#include "Weapon.hpp"
#include <stdio.h>

using namespace std;

extern Game game;

TEST(ShipModuleTest, FriendlyFire)
{
	game.restartTest();
	////Ship
	ChunkData* c = game.getUniverse().getBlueprints().getChunkSPtr("Anubis")->clone();
	c->bodyComp.coords = b2Vec2(2, 2);
	Chunk* testShip = c->generate(&game.getUniverse());
	testShip->getBodyComponent().setTeam(1);
	game.getUniverse().add(testShip);

	vector<sptr<Module> > modules = testShip->getModuleList();
	ShipModule* module = dynamic_cast<ShipModule*>(modules[0].get());
	int initialHealth = module->getHealth().getHealth();
	int modulePos = module->getFixtureComponent().getIOPos();
	Weapon::damage(&game.getUniverse().getUniverseIO(), modulePos, 1000, modulePos, 1);

	//Moved
	game.runTicks(2);
	EXPECT_EQ(module->getHealth().getHealth(), initialHealth);
}