#include "Random.hpp"
#include <time.h>

std::default_random_engine Random::gen;
std::uniform_int_distribution<int> Random::ints;
std::uniform_real_distribution<float> Random::floats;

void Random::seed()
{
	gen.seed(static_cast<int>(time(NULL)));
}
int Random::get(int minInclusive, int maxInclusive)
{
	std::uniform_int_distribution<int>::param_type range(minInclusive, maxInclusive);
	ints.param(range);
	return ints(gen);
}
float Random::get(float minInclusive, float maxExclusive)
{
	std::uniform_real_distribution<float>::param_type range(minInclusive, maxExclusive);
	floats.param(range);
	return floats(gen); 
}

