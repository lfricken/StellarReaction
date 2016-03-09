#include "stdafx.h"
#include "gtest/gtest.h"
#include "Stealth.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"
#include "Weapon.hpp"

extern Game game;

class BlueprintLoaderTest : public ::testing::Test
{
protected:
	virtual void setUp()
	{

	}

	virtual void tearDown()
	{

	}
};

TEST(BlueprintLoaderTest, loadBlueprint)
{
	game.runTicks(1);
	game.getUniverse().getBlueprints().storeWeapon("../StellarReactionTest/testblueprints/test_weapon_blueprint.bp");
	const WeaponData* const p = game.getUniverse().getBlueprints().getWeaponSPtr("test_blueprint").get();

	ASSERT_TRUE(p);

	EXPECT_EQ(p->damage, 400);
	EXPECT_EQ(p->collisions, 5);
	EXPECT_EQ(p->shots, 1);
}


