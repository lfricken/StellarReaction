#include "stdafx.h"
#include "gtest/gtest.h"
#include "Globals.hpp"
#include "Pool.hpp"

extern Game game;

typedef Pool<float> Poolf;
typedef PoolData<float> PoolDataf;

void initPoolParamTest(const vector<int>& cases)
{
	for(auto it = cases.cbegin(); it != cases.cend(); ++it)
	{
		PoolDataf data;
		data.Max = *it;
		data.Value = *it * 10.f;
		Poolf obj(data);
		EXPECT_EQ(*it, obj.getMax());
		EXPECT_EQ(*it, obj.getValue());
	}
};
TEST(Pool, MaxValueFloat)
{
	/**Parameters for test**/
	const int numCases = 300;
	vector<int> cases;
	for(int i = 0; i < numCases; ++i)
		cases.push_back(i + 100);

	initPoolParamTest(cases);
}
TEST(Pool, ChangeValueFloat)
{
	PoolDataf data;
	data.Max = 100.f;
	data.Min = 0.f;
	data.Value = 50.f;
	Poolf pool(data);

	float maxChange = -7.5f;
	float minChange = 5.4f;
	float change = 10.1f;

	pool.changeMax(maxChange);
	pool.changeMin(minChange);
	pool.changeValue(change);

	EXPECT_EQ(data.Max + maxChange, pool.getMax());
	EXPECT_EQ(data.Min + minChange, pool.getMin());
	EXPECT_EQ(data.Value + change, pool.getValue());
}







