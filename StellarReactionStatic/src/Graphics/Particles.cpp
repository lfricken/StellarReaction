#include "Particles.hpp"
#include "JSON.hpp"

void Particles::loadJson(const Json::Value& root)
{
	GETJSON(randDuration);
	GETJSON(duration);
	GETJSON(fadeInTime);
	GETJSON(fadeOutTime);
	GETJSON(number);
	GETJSON(randArc);
	GETJSON(velocity);
	GETJSON(randVelScalarMax);
	GETJSON(initialOrientation);
	GETJSON(minSpinRate);
	GETJSON(maxSpinRate);

	LOADJSON(quadData);
}
Particles* Particles::clone() const
{
	return new Particles(*this);
}