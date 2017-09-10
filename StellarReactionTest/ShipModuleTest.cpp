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


extern Game game;

TEST(ShipModuleTest, FriendlyFire)
{
	game.restartTest();
	////Ship
	ChunkData* c = game.getUniverse().getBlueprints().getChunkSPtr("Anubis")->clone();
	c->bodyComp.coords = b2Vec2(2, 2);
	Chunk* testShip = c->generate(&game.getUniverse());
	testShip->getBodyComponent().setTeam(Team::One);
	game.getUniverse().add(testShip);

	List<sptr<Module> > modules = testShip->getModuleList();
	ShipModule* module = dynamic_cast<ShipModule*>(modules[0].get());
	int initialHealth = module->getHealth().getHealth();
	int modulePos = module->getFixtureComponent().getIOPos();
	Weapon::damage(&game.getUniverse().getUniverseIO(), modulePos, 1000, modulePos, Team::One, Vec2(0,0), Vec2(0,0));

	//Make sure damage from the same team has no effect.
	game.runTicks(2);
	EXPECT_EQ(module->getHealth().getHealth(), initialHealth);
}