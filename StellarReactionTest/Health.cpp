#include "stdafx.h"
#include "gtest/gtest.h"
#include "Globals.hpp"
#include "Health.hpp"
#include "Random.hpp"

extern Game game;

TEST(Health, InitializeHealth)
{
	HealthData data;

	Random::seed();

	data.Armor = Random::getRandom(3, 10);
	data.Max = Random::getRandom(900, 2000);
	data.Min = 0;
	data.Value = Random::getRandom(500, 600);

	Health health(data);

	EXPECT_EQ(data.Armor, health.getArmor());
	EXPECT_EQ(data.Max, health.getMaxHealth());
	EXPECT_EQ(data.Value, health.getHealth());
	EXPECT_EQ(false, health.isDead());
}
TEST(Health, RecieveDamage)
{
	HealthData data;

	Random::seed();

	data.Armor = Random::getRandom(3, 10);
	data.Max = Random::getRandom(900, 2000);
	data.Min = 0;
	data.Value = Random::getRandom(500, 600);

	Health health(data);

	int damage = Random::getRandom(40, 50);
	int expectedHealth = data.Value - (damage - data.Armor);

	health.damage(damage);
	EXPECT_EQ(expectedHealth, health.getHealth());
	EXPECT_LT(0, health.getHealthPercent());
	EXPECT_GT(1, health.getHealthPercent());
}
TEST(Health, RecieveLowDamage)
{
	HealthData data;

	Random::seed();

	data.Armor = Random::getRandom(30, 50);
	data.Max = Random::getRandom(900, 2000);
	data.Min = 0;
	data.Value = data.Max;

	Health health(data);

	int damage = Random::getRandom(10, 20);
	int expectedHealth = data.Value;//because we did less damage then health

	health.damage(damage);
	EXPECT_EQ(expectedHealth, health.getHealth());
	EXPECT_EQ(1, health.getHealthPercent());
}
TEST(Health, DamageBeyondZero)
{
	HealthData data;

	Random::seed();

	data.Armor = Random::getRandom(0, 10);
	data.Max = Random::getRandom(100, 900);
	data.Min = 0;
	data.Value = data.Max;

	Health health(data);

	int damage = Random::getRandom(1000, 2000);
	int expectedHealth = data.Min;

	health.damage(damage);
	EXPECT_EQ(expectedHealth, health.getHealth());
	EXPECT_EQ(0, health.getHealthPercent());
}
TEST(Health, Healing)
{
	HealthData data;

	Random::seed();

	data.Armor = Random::getRandom(20, 30);
	data.Max = Random::getRandom(800, 900);
	data.Min = 0;
	data.Value = data.Max / 2;

	Health health(data);

	int heal = Random::getRandom(20, 30);
	int expectedHealth = data.Value + heal;

	health.heal(heal);
	EXPECT_EQ(expectedHealth, health.getHealth());
	EXPECT_LT(0, health.getHealthPercent());
}
TEST(Health, HealBeyondMax)
{
	HealthData data;

	Random::seed();

	data.Armor = Random::getRandom(0, 10);
	data.Max = Random::getRandom(100, 900);
	data.Min = 0;
	data.Value = data.Max;

	Health health(data);

	int heal = Random::getRandom(1000, 2000);
	int expectedHealth = data.Max;

	health.heal(heal);
	EXPECT_EQ(expectedHealth, health.getHealth());
	EXPECT_EQ(1, health.getHealthPercent());
}

