#include "Convert.hpp"

const int Convert::scale = 512;
const int Convert::standardModuleSize = 256;
const float Convert::universeToWorldFactor = static_cast<float>(Convert::standardModuleSize) / static_cast<float>(scale);
const float pi = 3.14159f;

