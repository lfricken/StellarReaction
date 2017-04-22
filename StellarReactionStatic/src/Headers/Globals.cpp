#include "Globals.hpp"

const String& contentDir()
{
	static const String dir("../content/");
	return dir;
}

const int scale = 512;//1 box2d unit is scale pixels at 1 zoom
const int standardModuleSize = 256;//size in pixels of module at 1 zoom
const float sizeScalingFactor = static_cast<float>(standardModuleSize) / static_cast<float>(scale);// = 0.5 atm
const float pi = 3.14159f;