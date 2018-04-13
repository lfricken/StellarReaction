
#include <math.h>
#include "UpgradeType.hpp"

void Upgrade::increase(UpgradeType type, float* value)
{
	*value = (float)ceilf((float)(*value) * getAmount());
}
void Upgrade::increase(UpgradeType type, int* value)
{
	*value = (int)ceilf((float)(*value) * getAmount());
}
float Upgrade::getAmount()
{
	float increase = 0.25;
	return 1 + increase;
}
