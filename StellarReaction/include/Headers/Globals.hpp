#ifndef GLOBALS_H
#define GLOBALS_H




#include "Game.hpp"
extern Game game;




const sf::Packet voidPacket;
const int scale = 512;
const int standardModuleSize = 256;
const float sizeScalingFactor = static_cast<float>(standardModuleSize) / static_cast<float>(scale);
const float pi = 3.14159f;
#define FILELINE "file:{"<<__FILE__<<"}line:{"<<__LINE__<<"}"



#endif // GLOBALS_H
