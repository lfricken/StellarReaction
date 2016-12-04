#include "Particles.hpp"
#include "JSON.hpp"

void Particles::loadJson(const Json::Value& root)
{
	GETJSON(duration);
	GETJSON(fadeTime);
	GETJSON(number);
	GETJSON(randRadArc);
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