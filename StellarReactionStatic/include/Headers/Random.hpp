#pragma once

#include <random>

/// \brief A class to generate Random numbers.
///
/// This has a bug described by this post:
/// http://stackoverflow.com/questions/36119190/static-random-number-generator-losing-seed-for-global-variable
class Random
{
public:
	/// Seed the random generator.
	static void seed();
	/// Get a random int between the given bounds.
	static int get(int minInclusive, int maxInclusive);
	/// Get a random float between the given bounds.
	static float get(float minInclusive, float maxExclusive);

private:
	static std::default_random_engine gen;
	static std::uniform_int_distribution<int> ints;
	static std::uniform_real_distribution<float> floats;
};

