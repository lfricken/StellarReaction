#include "stdafx.h"
#include "gtest/gtest.h"
#include "Stealth.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"
#include "Weapon.hpp"

extern Game game;

TEST(BlueprintLoaderTest, LoadWeapon)
{
	game.restartTest();

	game.getUniverse().getBlueprints().loadBlueprints("StellarReactionTest/testblueprints/");
	const WeaponData* const pWeaponData = game.getUniverse().getBlueprints().getWeaponSPtr("test_weapon").get();

	ASSERT_TRUE(pWeaponData);

	EXPECT_EQ(pWeaponData->energyConsumption, 1);
	EXPECT_EQ(pWeaponData->ballisticConsumption, 2);
	EXPECT_EQ(pWeaponData->missileConsumption, 3);

	EXPECT_EQ(pWeaponData->shots, 4);
	EXPECT_EQ(pWeaponData->shotsInSpread, 5);
	EXPECT_EQ(pWeaponData->fireArc, 6);
	EXPECT_EQ(pWeaponData->damage, 7);

	EXPECT_EQ(pWeaponData->shotDelay, 8);
	EXPECT_EQ(pWeaponData->fireDelay, 9);
	EXPECT_EQ(pWeaponData->range, 10);
	EXPECT_EQ(pWeaponData->collisions, 11);
}


