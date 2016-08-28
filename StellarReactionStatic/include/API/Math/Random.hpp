#pragma once

#include <random>

class Rand
{
public:
	static int get(int minInclusive, int maxExclusive);
	static float get(float minInclusive, float maxExclusive);
	static double get(double minInclusive, double maxExclusive);
};
