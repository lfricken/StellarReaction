#ifndef GLOBALS_H
#define GLOBALS_H


#include "Game.hpp"


const std::string& contentDir();//directory of content
const sf::Packet voidPacket;// a void packet when we don't want to send anything

//TODO these should be defined here, but set somewhere else so changing them doesn't trigger a total recompile
const int scale = 512;//1 box2d unit is scale pixels at 1 zoom
const int standardModuleSize = 256;//size in pixels of module at 1 zoom
const float sizeScalingFactor = static_cast<float>(standardModuleSize) / static_cast<float>(scale);// = 0.5 atm
const float pi = 3.14159f;
#define FILELINE "file:{"<<__FILE__<<"}line:{"<<__LINE__<<"}"

extern Game game;

#endif // GLOBALS_H
