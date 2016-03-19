#pragma once

#include <random>

class Random
{
public:

	static void seed();
	static int getRandom(int minInclusive, int maxInclusive);
	static float getRandom(float minInclusive, float maxExclusive);

private:
	static std::default_random_engine gen;
	static std::uniform_int_distribution<int> ints;
	static std::uniform_real_distribution<float> floats;
};

