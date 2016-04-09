#include "stdafx.h"
#include "gtest/gtest.h"
#include "Globals.hpp"
#include "Health.hpp"
#include "Random.hpp"

extern Game game;


void initHealthParamTest(const vector<int>& cases)
{
	for(auto it = cases.cbegin(); it != cases.cend(); ++it)
	{
		HealthData data;
		data.Max = *it;
		Health healthObj(data);
		EXPECT_EQ(*it, healthObj.getMaxHealth());
	}
};
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

	/**Parameters for test**/
	const int numCases = 300;
	vector<int> cases;
	for(int i = 0; i < numCases; ++i)
		cases.push_back(i + 100);

	initHealthParamTest(cases);
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
void damageHealthParamTest(const vector<int>& cases)
{
	HealthData data;
	data.Armor = 10;
	for(auto it = cases.cbegin(); it != cases.cend(); ++it)
	{
		Health healthObj(data);
		healthObj.damage(*it);
		if(*it < 0)
			EXPECT_EQ(healthObj.getMaxHealth(), healthObj.getHealth());
		else if(*it >= 110)
			EXPECT_EQ(0, healthObj.getHealth());
		else if(*it <= 10)
			EXPECT_EQ(healthObj.getMaxHealth(), healthObj.getHealth());
		else//in [11, 109]
			EXPECT_EQ(healthObj.getMaxHealth() + healthObj.getArmor() - *it, healthObj.getHealth());
	}
};
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

	/**Parameters for test**/
	const int numCases = 500;
	vector<int> cases;
	for(int i = 0; i < numCases; ++i)
		cases.push_back(i - 100);

	damageHealthParamTest(cases);// [-100, 400)
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

