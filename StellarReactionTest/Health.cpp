#include "stdafx.h"
#include "gtest/gtest.h"
#include "Globals.hpp"
#include "Health.hpp"


extern Game game;


void initHealthParamTest(const List<int>& cases)
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
	//Ensure health can be initialized with all sorts of values.
	HealthData data;

	data.Armor = Rand::get(3, 10);
	data.Max = Rand::get(900, 2000);
	data.Min = 0;
	data.Value = Rand::get(500, 600);

	Health health(data);

	EXPECT_EQ(data.Armor, health.getArmor());
	EXPECT_EQ(data.Max, health.getMaxHealth());
	EXPECT_EQ(data.Value, health.getHealth());
	EXPECT_EQ(false, health.isDead());

	/**Parameters for test**/
	const int numCases = 300;
	List<int> cases;
	for(int i = 0; i < numCases; ++i)
		cases.push_back(i + 100);

	initHealthParamTest(cases);
}

TEST(Health, RecieveDamage)
{
	//Check to see that damage is applied properly.
	HealthData data;

	data.Armor = Rand::get(3, 10);
	data.Max = Rand::get(900, 2000);
	data.Min = 0;
	data.Value = Rand::get(500, 600);

	Health health(data);

	int damage = Rand::get(40, 50);
	int expectedHealth = data.Value - (damage - data.Armor);

	health.damage(damage);
	EXPECT_EQ(expectedHealth, health.getHealth());
	EXPECT_LT(0, health.getHealthPercent());
	EXPECT_GT(1, health.getHealthPercent());
}
TEST(Health, NoSelfCrit)
{
	//Check to see that damage is applied properly.
	HealthData data;

	data.Armor = 0;
	data.Max = 1000;
	data.Min = 0;
	data.Value = 100;

	Health health(data);

	for(int i = 0; i < 1000; i++);//shouldn't crit at all
	{
		EXPECT_EQ(false, health.trySelfCrit());
	}

}
TEST(Health, SelfCrit)
{
	//Check to see that damage is applied properly.
	HealthData data;

	data.Armor = 0;
	data.Max = 1000;
	data.Min = 0;
	data.Value = 800;

	{
		Health health(data);
		health.incrementCritHits();

		int samples = 100000;
		int expected = samples * 0.3333f;
		int percentage = 2;//within 5%

		int critedSelf = 0;
		for(int i = 0; i < samples; i++)//shouldn't crit at all
		{
			if(health.trySelfCrit())
			{
				critedSelf++;
			}
		}

		EXPECT_LT(expected - percentage*expected / 100, critedSelf);
		EXPECT_GT(expected + percentage*expected / 100, critedSelf);
	}
	{
		Health health(data);
		health.incrementCritHits();
		health.incrementCritHits();

		int samples = 100000;
		int expected = samples * 0.6666;
		int percentage = 2;//within 5%

		int critedSelf = 0;
		for(int i = 0; i < samples; i++)//shouldn't crit at all
		{
			if(health.trySelfCrit())
			{
				critedSelf++;
			}
		}

		EXPECT_LT(expected - percentage*expected / 100, critedSelf);
		EXPECT_GT(expected + percentage*expected / 100, critedSelf);
	}

}
TEST(Health, TakeCritHit)
{
	//Check to see that damage is applied properly.
	HealthData data;

	data.Armor = 0;
	data.Max = 1000;
	data.Min = 0;
	data.Value = 1;

	data.MaxCritOdds = 0.3;
	int samples = 100000;
	float percentage = 2;

	int expected = samples * data.MaxCritOdds;

	Health health(data);

	int crited = 0;
	for(int i = 0; i < samples; i++)//shouldn't crit at all
	{
		if(health.tryHitCrit())
		{
			crited++;
		}
	}
	EXPECT_LT(expected - percentage*expected / 100, crited);//within 5%
	EXPECT_GT(expected + percentage*expected / 100, crited);//within 5%

}
TEST(Health, TakeNoCritHit)
{
	//Check to see that damage is applied properly.
	HealthData data;

	data.Armor = 0;
	data.Max = 1000;
	data.Min = 0;
	data.Value = 900;

	data.MaxCritOdds = 0.3;
	int samples = 100000;
	float percentage = 2;

	int expected = samples * data.MaxCritOdds;

	Health health(data);

	int crited = 0;
	for(int i = 0; i < samples; i++)//shouldn't crit at all
	{
		if(health.tryHitCrit())
		{
			crited++;
		}
	}
	EXPECT_EQ(0, crited);

}
TEST(Health, RecieveDamageLessThanArmor)
{
	//Ensure taking damage lower than armor amount will stop all of the damage.
	HealthData data;

	data.Armor = Rand::get(30, 50);
	data.Max = Rand::get(900, 2000);
	data.Min = 0;
	data.Value = data.Max;

	Health health(data);

	int damage = Rand::get(10, 20);
	int expectedHealth = data.Value;//because we did less damage then health

	health.damage(damage);
	EXPECT_EQ(expectedHealth, health.getHealth());
	EXPECT_EQ(1, health.getHealthPercent());
}
TEST(Health, DamageBeyondZero)
{
	//Ensure Health can't drop below 0.
	HealthData data;

	data.Armor = Rand::get(0, 10);
	data.Max = Rand::get(100, 900);
	data.Min = 0;
	data.Value = data.Max;

	Health health(data);

	int damage = Rand::get(1000, 2000);
	int expectedHealth = data.Min;

	health.damage(damage);
	EXPECT_EQ(expectedHealth, health.getHealth());
	EXPECT_EQ(0, health.getHealthPercent());
}
TEST(Health, Healing)
{
	//Make sure healing works as expected.
	HealthData data;

	data.Armor = Rand::get(20, 30);
	data.Max = Rand::get(800, 900);
	data.Min = 0;
	data.Value = data.Max / 2;

	Health health(data);

	int heal = Rand::get(20, 30);
	int expectedHealth = data.Value + heal;

	health.heal(heal);
	EXPECT_EQ(expectedHealth, health.getHealth());
	EXPECT_LT(0, health.getHealthPercent());
}
TEST(Health, HealBeyondMax)
{
	//Make sure we can't heal ourselves beyond max health.
	HealthData data;

	data.Armor = Rand::get(0, 10);
	data.Max = Rand::get(100, 900);
	data.Min = 0;
	data.Value = data.Max;

	Health health(data);

	int heal = Rand::get(1000, 2000);
	int expectedHealth = data.Max;

	health.heal(heal);
	EXPECT_EQ(expectedHealth, health.getHealth());
	EXPECT_EQ(1, health.getHealthPercent());
}

