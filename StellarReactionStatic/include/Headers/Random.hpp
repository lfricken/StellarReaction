#pragma once

#include <random>

class Random
{
public:
	Random();
	~Random();

	static int getRandom(int minInclusive, int maxInclusive);
	static float getRandom(float minInclusive, float maxExclusive);



private:
	static std::default_random_engine gen;

	static std::uniform_int_distribution<int> ints;
	static std::uniform_int_distribution<int>::param_type intRange;

	static std::uniform_real_distribution<float> floats;
	static std::uniform_real_distribution<float>::param_type floatRange;
};

