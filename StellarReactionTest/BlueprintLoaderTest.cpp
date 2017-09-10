#include "stdafx.h"
#include "gtest/gtest.h"
#include "Stealth.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"
#include "Projectile.hpp"
#include "Weapon.hpp"

extern Game game;

TEST(BlueprintLoaderTest, LoadWeapon)
{
	game.restartTest();

	//Load Weapon blueprint.
	game.getUniverse().getBlueprints().loadBlueprints("../StellarReactionTest/testblueprints/");
	const WeaponData* const pWeaponData = game.getUniverse().getBlueprints().getWeaponSPtr("test_weapon").get();

	//That bluprint has expected values now?
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
TEST(BlueprintLoaderTest, LoadModule)
{
	game.restartTest();

	//Load Module blueprint.
	game.getUniverse().getBlueprints().loadBlueprints("../StellarReactionTest/testblueprints/");
	const ModuleData* const pData = game.getUniverse().getBlueprints().getModuleSPtr("test_module").get();

	//That bluprint has expected values now?
	ASSERT_TRUE(pData);
	auto& data = *pData;

	String name = "DisplayName";
	EXPECT_EQ(data.name, name);
	EXPECT_EQ(data.collisionDamage, 11);
}
TEST(BlueprintLoaderTest, LoadChunk)
{
	game.restartTest();

	//Load Chunk Blueprint.
	game.getUniverse().getBlueprints().loadBlueprints("../StellarReactionTest/testblueprints/");
	const ChunkData* const pData = game.getUniverse().getBlueprints().getChunkSPtr("test_chunk").get();

	//That bluprint has expected values now?
	ASSERT_TRUE(pData);
	auto& data = *pData;

	EXPECT_EQ(data.energyData.Max, 1);
	EXPECT_EQ(data.energyData.Min, 2);
	EXPECT_EQ(data.energyData.Value, 3);

	EXPECT_EQ(data.ballisticData.Max, 4);
	EXPECT_EQ(data.ballisticData.Min, 5);
	EXPECT_EQ(data.ballisticData.Value, 6);

	EXPECT_EQ(data.missileData.Max, 7);
	EXPECT_EQ(data.missileData.Min, 8);
	EXPECT_EQ(data.missileData.Value, 9);

	EXPECT_EQ(data.zoomData.Max, 10);
	EXPECT_EQ(data.zoomData.Min, 11);
	EXPECT_EQ(data.zoomData.Value, 12);

	EXPECT_EQ(data.moduleData.size(), 3);
}
TEST(BlueprintLoaderTest, LoadBody)
{
	game.restartTest();

	game.getUniverse().getBlueprints().loadBlueprints("../StellarReactionTest/testblueprints/");
	const ProjectileData* const pData = game.getUniverse().getBlueprints().getProjectileSPtr("test_projectile").get();

	ASSERT_TRUE(pData);
	auto& data = *pData;


	EXPECT_EQ(1, data.body.coords.x);
	EXPECT_EQ(2, data.body.coords.y);

	EXPECT_EQ(3, data.body.rotation);
	EXPECT_EQ(true, data.body.isDynamic);
	EXPECT_EQ(true, data.body.isBullet);
	EXPECT_EQ(true, data.body.startAwake);
}


