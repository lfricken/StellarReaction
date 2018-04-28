
#include <math.h>
#include "UpgradeType.hpp"

void Upgrade::increase(UpgradeType type, float* value, float initValue, int level)
{
	*value = (float)ceilf((float)(*value) * getAmount(level));
}
void Upgrade::increase(UpgradeType type, int* value, int initValue, int level)
{
	*value = (int)ceilf((float)(*value) * getAmount(level));
}
float Upgrade::getAmount(int level)
{
	const float increasePerLevel = 0.25f;
	return (float)powf(1 + increasePerLevel, (float)level);
}
